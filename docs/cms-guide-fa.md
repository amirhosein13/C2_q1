# راهنمای قدم‌به‌قدم برگزاری آزمون با CMS (از صفر)

این راهنما برای **CMS نسخه‌ی 1.5.1** (آخرین نسخه‌ی پایدار، ژوئن ۲۰۲۵) روی **Ubuntu 24.04** نوشته شده است.
دستورها و گزینه‌ها از مستندات و کد خود CMS 1.5.1 برداشته شده‌اند. اسم دکمه‌ها و فیلدهای صفحه‌ی ادمین را از روی
کد ندیده‌ام و ممکن است کمی فرق کنند؛ هر جا این‌طور است با «(تقریبی)» مشخص کرده‌ام.

---

## ۰. CMS از چه تکه‌هایی ساخته شده؟

CMS یک برنامه‌ی واحد نیست؛ چند «سرویس» است که با هم کار می‌کنند:

| سرویس | کار | پورت پیش‌فرض |
|---|---|---|
| PostgreSQL | پایگاه داده: همه‌چیز (کاربرها، سوال‌ها، ارسال‌ها) اینجاست | 5432 |
| LogService | جمع‌کردن لاگ همه‌ی سرویس‌ها | — |
| ResourceService | روشن‌کردن و زنده‌نگه‌داشتن بقیه‌ی سرویس‌ها روی هر ماشین | — |
| AdminWebServer (AWS) | **صفحه‌ی ادمین**؛ ساخت آزمون، تنظیمات، دیدن ارسال‌ها | **8889** |
| ContestWebServer (CWS) | **صفحه‌ی شرکت‌کننده‌ها**؛ ورود، دیدن سوال، ارسال | **8888** |
| EvaluationService + Worker | کامپایل و اجرای کد شرکت‌کننده‌ها در sandbox (isolate) | — |
| ScoringService | حساب‌کردن امتیاز | — |
| RankingWebServer (RWS) | جدول رده‌بندی زنده (اختیاری) | 8890 |

شما عملاً با دو چیز کار دارید: **ترمینال** (برای نصب، import سوال، ساخت کاربر) و **صفحه‌ی ادمین روی پورت 8889**.

---

## ۱. سرور

برای آزمون هفتگی آنلاین با چند ده نفر یک سرور کافی است:

- Ubuntu 24.04 (سرور مجازی یا فیزیکی)، دسترسی root یا sudo.
- حداقل ۴ هسته و ۸ گیگابایت RAM. هر Worker یک ارسال را هم‌زمان داوری می‌کند؛ با ۴ هسته ۲ تا ۳ Worker منطقی است.
- **swap خاموش** باشد (وگرنه محدودیت حافظه درست اعمال نمی‌شود): `sudo swapoff -a` و خط swap را از `/etc/fstab` حذف کنید.
- کرنل با cgroup v2 (در Ubuntu 24.04 پیش‌فرض همین است).
- یک IP عمومی یا دامنه، اگر شرکت‌کننده‌ها از بیرون وصل می‌شوند.

> اگر فقط می‌خواهید **تمرین** کنید، می‌توانید اول همه‌ی مراحل را روی یک ماشین مجازی محلی انجام دهید.

---

## ۲. نصب پیش‌نیازها

```bash
sudo apt-get update
sudo apt-get install build-essential openjdk-11-jdk-headless fp-compiler \
    postgresql postgresql-client python3.12 cppreference-doc-en-html \
    cgroup-lite libcap-dev zip
# چون CMS را با venv نصب می‌کنیم:
sudo apt-get install python3.12-dev python3.12-venv libpq-dev libcups2-dev libyaml-dev \
    libffi-dev python3-pip
# اختیاری (برای HTTPS):
sudo apt-get install nginx-full
```

---

## ۳. دانلود و نصب CMS

```bash
cd ~
git clone --recursive --branch v1.5.1 https://github.com/cms-dev/cms.git
cd cms
sudo python3 prerequisites.py install
```

`prerequisites.py` این کارها را انجام می‌دهد: sandbox (`isolate`) را کامپایل می‌کند، کاربر و گروه `cmsuser` را می‌سازد و
فایل‌های تنظیم را در `/usr/local/etc/` کپی می‌کند.
وقتی پرسید «شما را به گروه cmsuser اضافه کنم؟» بگویید **Y**. بعد **یک بار logout و دوباره login کنید** و با `groups`
مطمئن شوید `cmsuser` در لیست هست.

> هشدار: هر کسی در گروه `cmsuser` باشد می‌تواند root شود. هیچ کاربر غیرقابل‌اعتمادی را به این گروه اضافه نکنید.

حالا خود CMS را در یک virtual environment نصب کنید:

```bash
python3 -m venv ~/cms_venv
source ~/cms_venv/bin/activate        # هر بار که ترمینال جدید باز می‌کنید این را بزنید
cd ~/cms
pip install -r requirements.txt
pip install .
```

بعد از این، دستورهایی مثل `cmsInitDB`، `cmsAddAdmin` و `cmsImportTask` در دسترس‌اند (به شرط فعال بودن venv).

---

## ۴. پایگاه داده

```bash
sudo su - postgres
createuser --username=postgres --pwprompt cmsuser          # یک رمز بگذارید و یادداشت کنید
createdb --username=postgres --owner=cmsuser cmsdb
psql --username=postgres --dbname=cmsdb --command='ALTER SCHEMA public OWNER TO cmsuser'
psql --username=postgres --dbname=cmsdb --command='GRANT SELECT ON pg_largeobject TO cmsuser'
logout
```

---

## ۵. فایل تنظیمات `cms.conf`

فایل معمولاً در `/usr/local/etc/cms.conf` است (JSON؛ خط‌هایی که با `"_help"` شروع می‌شوند توضیح‌اند).

```bash
sudo nano /usr/local/etc/cms.conf
```

حداقل این‌ها را عوض کنید:

1. **`database`**: رمزی که در مرحله‌ی ۴ گذاشتید:
   ```
   "database": "postgresql+psycopg2://cmsuser:رمز_شما@localhost:5432/cmsdb",
   ```
2. **`secret_key`**: یک کلید تصادفی بسازید و جایگزین کنید (اگر پیش‌فرض بماند، شرکت‌کننده‌ها می‌توانند جای هم وارد شوند!):
   ```bash
   python -c 'from cmscommon import crypto; print(crypto.get_hex_random_key())'
   ```
3. **`Worker`** در `core_services`: یک خط به ازای هر Worker، با پورت‌های متفاوت. برای ۲ Worker:
   ```
   "Worker": [["localhost", 26000], ["localhost", 26001]],
   ```
4. **`rankings`**: نام کاربری و رمز `usern4me:passw0rd` را عوض کنید (و همان را در `/usr/local/etc/cms.ranking.conf` بگذارید).
5. **`admin_listen_address`**: پیش‌فرض `0.0.0.0` است یعنی صفحه‌ی ادمین از اینترنت هم باز است. امن‌تر است
   آن را `127.0.0.1` بگذارید و با SSH tunnel وصل شوید (پایین‌تر توضیح داده‌ام).
6. `keep_sandbox` را `false` نگه دارید (وگرنه دیسک پر می‌شود).

سپس جدول‌های پایگاه داده را بسازید و یک ادمین اضافه کنید:

```bash
cmsInitDB
cmsAddAdmin admin            # یک رمز تصادفی چاپ می‌کند؛ یادداشت کنید
```

---

## ۶. روشن‌کردن صفحه‌ی ادمین و ساخت آزمون

ResourceService باید بداند کدام آزمون را اجرا کند، ولی هنوز آزمونی نساخته‌ایم. پس اول **فقط** صفحه‌ی ادمین را روشن کنید:

```bash
cmsAdminWebServer
```

اگر `admin_listen_address` را `127.0.0.1` گذاشته‌اید، از کامپیوتر خودتان یک SSH tunnel بزنید:

```bash
ssh -L 8889:localhost:8889 user@SERVER_IP
```

و در مرورگر `http://localhost:8889` را باز کنید. با `admin` و رمز مرحله‌ی ۵ وارد شوید.

**ساخت آزمون (در صفحه‌ی ادمین، تقریبی):** از منوی سمت چپ گزینه‌ی ساخت contest را بزنید و این‌ها را پر کنید:

- **Name**: یک شناسه‌ی کوتاه، مثلاً `gold-w01`؛ **Description**: عنوان نمایشی.
- **Allowed languages**: فقط `C++17 / g++` یا `C++20 / g++` را تیک بزنید (سوال ما فقط grader C++ دارد).
- **Start / Stop**: زمان شروع و پایان. **Timezone**: `Asia/Tehran`.
- **Per-user time** (اختیاری): اگر هر نفر باید از لحظه‌ی ورودش مثلاً ۵ ساعت وقت داشته باشد (مثل آزمون‌های آنلاین با شروع آزاد).
- **Tokens**: روی disabled/infinite بگذارید (در سبک IOI توکن نداریم).
- **Score precision**: 2.
- اجازه‌ی پرسش و پاسخ (questions) را اگر می‌خواهید روشن کنید.

بعد از ذخیره، **ID آزمون** را ببینید (در آدرس صفحه، مثلاً `/contest/1` یعنی ID = 1).

حالا `cmsAdminWebServer` را با Ctrl+C ببندید و همه‌ی سرویس‌ها را روشن کنید (در دو ترمینال جدا، یا در `tmux`/`screen`
تا با بستن SSH خاموش نشوند):

```bash
# ترمینال ۱
source ~/cms_venv/bin/activate
cmsLogService

# ترمینال ۲
source ~/cms_venv/bin/activate
cmsResourceService -a 1        # 1 = ID آزمون
```

باید لاگ سرویس‌ها در ترمینال ۱ ظاهر شود. حالا صفحه‌ی ادمین (8889) و صفحه‌ی شرکت‌کننده‌ها (8888) هر دو روشن‌اند.

---

## ۷. آماده‌کردن و import سوال

### ۷.۱ ساخت بسته روی کامپیوتر خودتان (جایی که ریپو هست)

```bash
cd C2_q1/lice
tps gen                                   # تست‌ها را می‌سازد (حدود ۱ دقیقه)
tps export CMS 1 -o lice-cms -a zip       # فقط protocol 1 با CMS سازگار است
```

متن سوال را exporter خودش نمی‌گذارد؛ دستی اضافه کنید:

```bash
unzip lice-cms.zip                         # پوشه‌ی lice/ ساخته می‌شود (داخل پوشه‌ی دیگری از lice تا قاطی نشود)
mkdir -p lice/statements
cp statement/lice-fa.pdf lice/statements/fa.pdf
zip -r lice-final.zip lice
scp lice-final.zip user@SERVER_IP:~
```

> نکته: بهتر است `unzip` را در یک پوشه‌ی خالی جدا انجام دهید تا پوشه‌ی خروجی با خود پوشه‌ی سوال قاطی نشود.

### ۷.۲ import روی سرور

```bash
source ~/cms_venv/bin/activate
cd ~ && unzip lice-final.zip
cmsImportTask -L tps_task lice -c 1        # -c = ID آزمون
```

این دستور checker را روی سرور کامپایل می‌کند (پس `g++` روی سرور لازم است)، تست‌ها، grader و پیوست
`lice.zip` (grader نمونه + مثال‌ها برای دانلود شرکت‌کننده‌ها) را بارگذاری می‌کند.

برای به‌روزرسانی بعدی همان سوال: `cmsImportTask -L tps_task -u lice -c 1`

### ۷.۳ تنظیمات دستی بعد از import (مهم!)

در صفحه‌ی ادمین، صفحه‌ی سوال `lice` را باز کنید:

1. **Score mode** را روی **`max_subtask`** بگذارید (= سبک IOI: برای هر زیرمسئله بهترین ارسال حساب می‌شود).
   loader نسخه‌ی 1.5.1 این را تنظیم نمی‌کند و پیش‌فرض CMS چیز دیگری است (`max_tokened_last`).
2. **Primary statement**: زبان `fa` را انتخاب کنید (loader فقط `en_US` را خودکار primary می‌کند).
3. **Feedback level** (اگر هست): `full` یعنی شرکت‌کننده نتیجه‌ی هر تست را می‌بیند؛ `restricted` فقط نتیجه‌ی زیرمسئله‌ها را.
   برای آزمون آموزشی هفتگی معمولاً `full` یا نتیجه‌ی زیرمسئله‌ها کافی است.
4. محدودیت ارسال: loader این‌ها را می‌گذارد: حداکثر **۵۰ ارسال** و **۶۰ ثانیه** فاصله بین دو ارسال. اگر می‌خواهید عوض کنید.
5. در بخش **dataset** چک کنید:
   - Time limit = 2، Memory limit = 256 MiB
   - Task type = Batch با grader، و ارزیابی با `comparator` (یعنی checker)
   - Managers: `checker`، `grader.cpp`، `lice.h`
   - ۷۱ testcase
   - Score type = GroupMin با ۶ ردیف (۰، ۱۰، ۱۲، ۱۸، ۲۵، ۳۵) با جمع ۱۰۰

---

## ۸. ساخت کاربرها (شرکت‌کننده‌ها)

برای هر نفر دو دستور: ساخت کاربر، و اضافه‌کردن او به آزمون.

```bash
cmsAddUser -p رمز Ali Ahmadi ali01
cmsAddParticipation -c 1 ali01
```

برای تعداد زیاد، یک فایل `users.csv` بسازید (`first,last,username,password`) و:

```bash
while IFS=, read -r first last user pass; do
  cmsAddUser -p "$pass" "$first" "$last" "$user"
  cmsAddParticipation -c 1 "$user"
done < users.csv
```

برای خودتان هم یک کاربر تستی بسازید (مثلاً `tester`) و با `--hidden` اضافه کنید تا در رده‌بندی دیده نشود:

```bash
cmsAddUser -p test123 Test User tester
cmsAddParticipation -c 1 --hidden tester
```

---

## ۹. آزمایش قبل از آزمون (حتماً انجام دهید)

1. **محیط داوری**: اسکریپت `isolate-check-environment` را از
   [مخزن isolate](https://github.com/ioi/isolate/blob/master/isolate-check-environment) دانلود و اجرا کنید و
   هشدارهایش را برطرف کنید (swap، CPU frequency scaling، و ...).
2. با کاربر `tester` در `http://SERVER_IP:8888` وارد شوید. برای این تست، زمان شروع آزمون را موقتاً به گذشته ببرید.
3. **ارسال راه‌حل اصلی** (`solution/model.cpp`): باید ۱۰۰ بگیرد.
4. **ارسال چند راه‌حل دیگر** و مقایسه با جدول `lice/README.md`:
   - `slow_n2.cpp` → ۵۳
   - `brute.cpp` → ۱۰
   - `wrong_le2.cpp` → ۰
5. **زمان‌ها** را در صفحه‌ی ارسال در پنل ادمین ببینید:
   - روی ماشین من بیشترین زمان راه‌حل اصلی 0.50 ثانیه بود (با محدودیت ۲ ثانیه).
   - اگر روی سرور شما بیشتر از حدود 0.8 ثانیه شد، یا یک راه‌حل کند به‌جای TLE قبول شد، محدودیت زمان را بازبینی کنید.
   - راه بازبینی: در پنل ادمین dataset را **clone** کنید، TL را در کپی عوض کنید و آن را **active** کنید.
     راه دیگر: `problem.json` را عوض کنید و دوباره import کنید.
6. **فایل پیوست**: باید بتوانید `lice.zip` را از صفحه‌ی سوال دانلود کنید. با `compile_cpp.sh` و grader نمونه‌ی داخلش
   روی مثال اجرا کنید.
7. **متن سوال**: PDF فارسی باید باز شود.
8. بعد از تست:
   - زمان شروع را درست کنید.
   - ارسال‌های `tester` چون hidden است در رده‌بندی دیده نمی‌شود.

---

## ۱۰. دسترسی شرکت‌کننده‌ها از اینترنت (HTTPS)

ساده‌ترین حالت: شرکت‌کننده‌ها به `http://SERVER_IP:8888` وصل می‌شوند. برای آزمون واقعی بهتر است nginx با HTTPS
جلوی CWS بگذارید:

- نمونه‌ی تنظیم: `~/cms/config/nginx.conf.sample`.
- گواهی رایگان: با certbot و Let's Encrypt.
- اگر nginx جلوی CWS است، در `cms.conf` مقدار `num_proxies_used` را `1` کنید (تا IP واقعی کاربر دیده شود).
- فایروال: فقط پورت‌های 80/443 (و 22 برای SSH) را باز بگذارید. 8889 (ادمین) را از بیرون باز نکنید.

---

## ۱۱. رده‌بندی زنده (اختیاری)

```bash
cmsRankingWebServer      # در یک ترمینال دیگر؛ روی پورت 8890
```

نام کاربری و رمز آن باید با `rankings` در `cms.conf` یکی باشد.

---

## ۱۲. حین آزمون

- **لاگ‌ها**: ترمینال `cmsLogService`. هشدارها و خطاها روی صفحه‌ی اول پنل ادمین هم نمایش داده می‌شوند.
- **سوال‌ها و اطلاعیه‌ها**: از پنل ادمین می‌توانید به questionها جواب دهید و announcement بفرستید.
- **صف داوری**: در پنل ادمین وضعیت ارسال‌ها دیده می‌شود. اگر صف طولانی شد، Worker اضافه کنید (خط جدید در `cms.conf`
  و ری‌استارت ResourceService).
- **وقت اضافه** برای یک نفر: `cmsAddParticipation` گزینه‌ی `-e` (ثانیه) دارد؛ از پنل ادمین هم می‌شود.
- **داوری مجدد** (مثلاً بعد از اصلاح checker/تست): از پنل ادمین، روی سوال یا dataset گزینه‌ی re-evaluate/rejudge (تقریبی).

---

## ۱۳. بعد از آزمون

- رده‌بندی را از پنل ادمین (صفحه‌ی ranking) دانلود کنید (CSV/TXT؛ تقریبی).
- **نسخه‌ی پشتیبان کامل**:
  ```bash
  cmsDumpExporter backup-gold-w01.tar.gz
  ```
- برای آزمون هفته‌ی بعد: آزمون جدید بسازید (ID جدید)، سوال‌ها را با `-c` همان ID import کنید، کاربرها را با
  `cmsAddParticipation -c <ID جدید>` اضافه کنید (کاربرها دوباره ساخته نمی‌شوند)، و ResourceService را با ID جدید اجرا کنید.

---

## ۱۴. مشکلات رایج

| علامت | علت محتمل |
|---|---|
| `cmsInitDB` خطای اتصال می‌دهد | رمز یا نام در `database` داخل `cms.conf` اشتباه است |
| ارسال‌ها در صف می‌مانند و داوری نمی‌شوند | Worker یا EvaluationService روشن نیست؛ لاگ را ببینید؛ کاربر در گروه `cmsuser` نیست یا بعد از اضافه‌شدن logout نکرده |
| همه‌ی ارسال‌ها Compilation failed | زبان مجاز آزمون با grader جور نیست (فقط C++ را فعال کنید) |
| همه‌ی ارسال‌ها ۰ با پیام checker | checker روی سرور کامپایل نشده (در خروجی `cmsImportTask` نگاه کنید) |
| امتیاز کل عجیب است | Score mode روی `max_subtask` نیست (مرحله‌ی ۷.۳) |
| شرکت‌کننده‌ها از جای دیگری لاگین می‌شوند | `secret_key` عوض نشده |
| زمان‌ها خیلی متغیرند | `isolate-check-environment` را اجرا کنید؛ swap و CPU scaling را خاموش کنید |

---

## خلاصه‌ی دستورها (برای دفعه‌های بعد)

```bash
# روی کامپیوتر خودتان
cd C2_q1/<task> && tps gen && tps export CMS 1 -o <task>-cms -a zip
# + اضافه‌کردن statements/fa.pdf و scp به سرور

# روی سرور
source ~/cms_venv/bin/activate
cmsImportTask -L tps_task <task> -c <ID>          # بعد: score mode = max_subtask، primary statement = fa
cmsAddUser -p PASS First Last user && cmsAddParticipation -c <ID> user
cmsLogService            # ترمینال ۱
cmsResourceService -a <ID>   # ترمینال ۲
```

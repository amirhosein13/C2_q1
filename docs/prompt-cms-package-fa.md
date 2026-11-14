# پرامپت: آماده‌کردن یک سوال TPS برای آپلود دستی در CMS (فقط پنل ادمین)

این متن را در چت‌هایی بفرستید که سوالشان در TPS آماده است (یا تقریباً آماده است):

```
سوال این چت باید برای آپلود در CMS آماده شود. من فقط دسترسی پنل ادمین CMS را دارم (نسخه‌ی 1.5.1)،
نه ترمینال سرور؛ پس cmsImportTask قابل استفاده نیست و همه‌چیز باید دستی از پنل آپلود شود.

مرجع: ریپوی github.com/amirhosein13/c2_q1، شاخه‌ی claude/olympiad-exam-question-design-qxwo7p.
سوال lice/ در آن کامل و تأییدشده است. اگر فایل‌های زیر در ریپوی تو نیستند، از همان‌جا بردار
(با add_repo یا git clone) و بدون تغییرِ بی‌دلیل در ریپوی خودت بگذار:
  - tools/make_cms_upload.py    (ساخت بسته‌ی آپلود)
  - tools/statement/            (ساخت PDF فارسی RTL: package.json + build.js)
  - docs/cms-admin-only-fa.md   (راهنمای آپلود از پنل)
  - CLAUDE.md                   (قراردادها و درس‌ها؛ اول این را بخوان)

کارها، به ترتیب:
1. وضعیت فعلی سوال را چک کن: tps gen، tps verify، tps invoke برای همه‌ی راه‌حل‌ها (همه "match with
   expected")، تست checker. اگر چیزی ناقص یا خراب است اول درستش کن و بگو چه بود.
2. متن فارسی statement/index.md را به PDF تبدیل کن:
   node tools/statement/build.js <task>/statement/index.md <task>/statement/<task>-fa.pdf
   و صفحه‌ها را به تصویر تبدیل کن و خودت نگاه کن (راست‌به‌چپ، فرمول‌ها، جدول زیرمسئله‌ها، نمونه‌ها).
3. بسته‌ی آپلود را بساز (داخل پوشه‌ی سوال، بعد از tps gen):
   python3 ../tools/make_cms_upload.py statement/<task>-fa.pdf
   و خروجی <task>/cms-upload/ را بررسی کن.
4. سازگاری با CMS 1.5.1 را واقعاً چک کن، نه فرضی:
   - tps export CMS 1 (فقط protocol 1؛ protocol 2 با loader رسمی خطا می‌دهد).
   - loader رسمی tps_task نسخه‌ی v1.5.1 را از github.com/cms-dev/cms روی خروجی export اجرا کن
     (اسکریپت tools/cms_loader_dryrun.py در ریپوی مرجع همین کار را بدون پایگاه داده می‌کند).
   - checker و grader+راه‌حل اصلی با -std=gnu++14 کامپایل شوند (CMS 1.5.1 checker را با gnu++14 می‌سازد).
   - checker در بسته باید فایل اجرایی static x86-64 با نام دقیق "checker" باشد؛ grader با نام
     grader.cpp و هدر <task>.h (برای Batch با grader).
   - هر zip تست زیر 100MB باشد (حد پیش‌فرض آپلود پنل)؛ اسم تست‌ها <codename>.in / <codename>.out با
     الگوهای *.in و *.out.
   - پارامترهای GroupMin با regex لنگردار ^(...)$ (CMS از regex.match استفاده می‌کند)، و چک کن هر
     regex دقیقاً همان تعداد تست زیرمسئله را بگیرد و جمع امتیاز 100 باشد.
   - اگر نوع سوال Batch نیست (تعاملی/Communication، OutputOnly، TwoSteps)، make_cms_upload.py را برای
     آن نوع گسترش بده (manager کامپایل‌شده، stub، پارامترهای task type) و مقادیر را از کد
     cms/grading/tasktypes/ و قالب‌های cms/server/admin/templates/ نسخه‌ی 1.5.1 بردار، نه از حدس.
5. فایل SETTINGS.md داخل cms-upload باید مقدار تک‌تک فیلدهای پنل را داشته باشد، از جمله:
   Score mode = «...IOI 2017-» (max_subtask؛ پیش‌فرض CMS چیز دیگری است)، primary statement = fa،
   Task type و پارامترهایش، TL و حافظه (MiB)، managers، score type و پارامترها، الگوهای تست، تعداد تست.
6. cms-upload را در .gitignore بگذار (حجیم است)، بقیه را commit و push کن روی شاخه‌ی کاری خودت.
7. فایل‌های کوچک بسته (SETTINGS.md، score_type_parameters.json، PDF، پیوست zip، managers) را برایم
   بفرست. برای zipهای تست بگو چطور روی کامپیوتر خودم بسازم (tps gen + make_cms_upload.py؛ اگر
   tps/ خالی است: git submodule update --init).
8. در پایان به فارسی و کوتاه بگو:
   - نوع سوال، زیرمسئله‌ها، TL/ML
   - جدول invoke (هر راه‌حل × هر زیرمسئله) و زمان بدترین تست راه‌حل اصلی
   - مراحل آپلود در پنل ادمین، قدم‌به‌قدم، مخصوص همین سوال
   - جدول ارسال‌های آزمایشی بعد از آپلود (کدام فایل → چه امتیازی باید بگیرد)
   - هر چیزی که تأیید نکردی را صریح بگو.
```

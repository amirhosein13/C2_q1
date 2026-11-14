# پرامپت: آماده‌کردن سوالِ همین چت برای آپلود دستی در CMS (فقط پنل ادمین)

این نسخه مستقل است: به هیچ ریپو یا سوال دیگری (از جمله `lice`) ارجاع نمی‌دهد.

```
فقط روی سوالِ خودِ این چت کار کن. از هیچ ریپو، سوال یا چت دیگری فایل، کد، تست یا متن کپی نکن.
اگر قبلاً چیزی مربوط به سوال دیگری (مثلاً پوشه یا فایل‌هایی با نام lice، متن، تست، grader، checker یا
بسته‌ی آپلود آن) به ریپوی این چت اضافه کرده‌ای، اول همه را پیدا و حذف کن (یا commit مربوطه را revert کن)
و بگو دقیقاً چه چیزهایی بود.

هدف: سوالِ این چت را طوری آماده کن که بتوانم آن را دستی از پنل ادمین CMS آپلود کنم.
- من فقط دسترسی پنل ادمین (AdminWebServer) دارم، نه ترمینال سرور؛ پس cmsImportTask در دسترس نیست.
- نسخه‌ی CMS: 1.5.1.
- هر چیزی درباره‌ی CMS را از کد خود CMS تأیید کن، نه از حافظه:
  git clone --depth 1 --branch v1.5.1 https://github.com/cms-dev/cms
  (به‌خصوص cmscontrib/loaders/tps.py، cms/grading/tasktypes/، cms/grading/scoretypes/،
  cmscommon/importers.py و قالب‌های cms/server/admin/templates/).

کارها، به ترتیب:

1. بررسی سلامت سوال:
   tps gen، tps verify، و tps invoke برای همه‌ی راه‌حل‌ها (همه باید "match with expected" باشند)،
   و اجرای checker روی چند خروجی دست‌ساز (درست، غلط، فرمت غلط، ناقص، اضافه).
   اگر چیزی خراب یا ناقص است، اول درستش کن و گزارش بده.

2. PDF فارسی متن سوالِ همین چت، از Markdown موجود در statement/:
   - راست‌به‌چپ و با فونت فارسی؛ فرمول‌ها رندر شوند.
   - با هر ابزاری که در محیطت کار می‌کند (مثلاً markdown → HTML با KaTeX و چاپ با Chromium/Playwright).
   - صفحه‌ها را به تصویر تبدیل کن و خودت نگاه کن: جهت متن، فرمول‌ها، جدول زیرمسئله‌ها، نمونه‌ها.

3. ساخت بسته‌ی آپلود در <task>/cms-upload/ (با یک اسکریپت تکرارپذیر در ریپو).
   خروجی را از `tps export CMS 1 -a none` بگیر؛ فقط protocol 1، چون loader رسمی CMS protocol 2 را نمی‌فهمد.
   محتوای بسته:
   - managers/ — دقیقاً همان فایل‌هایی که نوع سوال در CMS 1.5.1 لازم دارد، با اسم‌هایی که CMS انتظار دارد:
     • Batch با grader: grader.cpp و <task>.h.
     • checker به‌صورت فایل اجرایی static لینوکس x86-64 با نام دقیق checker، کامپایل‌شده با
       g++ -x c++ -std=gnu++14 -O2 -static.
     • Communication/تعاملی، OutputOnly یا TwoSteps: manager و stub و پارامترها را دقیقاً از
       cms/grading/tasktypes/ همان نسخه دربیاور.
   - testcases-partN.zip — همه‌ی تست‌ها به‌صورت فایل‌های تخت <codename>.in و <codename>.out (بدون پوشه).
     هر zip زیر ۷۰MB باشد، چون پنل به‌طور پیش‌فرض بدنه‌ی بزرگ‌تر از ۱۰۰MB را رد می‌کند.
   - score_type_parameters.json — پارامترهای GroupMin به شکل [[امتیاز, "regex"], ...] به ترتیب زیرمسئله‌ها.
     • regex لنگردار ^(...)$ باشد، چون CMS از regex.match استفاده می‌کند.
     • با یک اسکریپت چک کن که هر regex دقیقاً تست‌های همان زیرمسئله را بگیرد و جمع امتیازها ۱۰۰ باشد.
   - attachments/<task>.zip — بسته‌ی عمومی شرکت‌کننده‌ها (خروجی tps make-public).
     مطمئن شو هیچ داده‌ی مخفی، مثل رشته‌ی مخفی خروجی، در آن نیست.
   - statement-fa.pdf
   - SETTINGS.md — مقدار تک‌تک فیلدهای پنل برای همین سوال:
     • Name، Title، statement با کد زبان fa و تیک Primary، پیوست، Submission format،
       Feedback level، Tokens غیرفعال، محدودیت ارسال، Score precision.
     • Score mode = «Use the sum over each subtask of the best result ... (IOI 2017-)»؛ پیش‌فرض CMS این نیست.
     • Time limit (ثانیه) و Memory limit (MiB).
     • Task type و همه‌ی پارامترهایش با همان برچسب‌هایی که در فرم پنل دیده می‌شود.
     • Managers.
     • Score type = GroupMin و پارامترهایش.
     • آپلود تست‌ها با Add multiple testcases و الگوهای *.in و *.out، و تعداد نهایی تست‌ها.
   cms-upload/ را در .gitignore بگذار، چون حجیم است.

4. تأیید واقعی سازگاری با CMS 1.5.1:
   - loader رسمی tps_task همان نسخه را روی خروجی export اجرا کن. اگر نصب کامل CMS ممکن نیست،
     ماژول‌های cms.* را stub کن و فقط loader را اجرا کن. خروجی (task type و پارامترها، managers،
     تعداد تست، score type) را با SETTINGS.md مقایسه کن.
   - grader داوری + راه‌حل اصلی را با -std=gnu++14 کامپایل کن.
   - هر موردی که تأیید نشد را صریح بگو.

5. commit و push روی شاخه‌ی کاری همین چت (نه main).

6. فایل‌های کوچک بسته را برایم بفرست: SETTINGS.md، score_type_parameters.json، PDF، پیوست zip و managers.
   برای zipهای تست بگو چطور روی کامپیوتر خودم بسازم: کدام دستورها، از کدام پوشه، و اگر پوشه‌ی TPS خالی
   بود (submodule)، چطور پرش کنم.

7. در پایان، به فارسی و کوتاه:
   - نوع سوال، زیرمسئله‌ها، TL و ML.
   - جدول invoke (هر راه‌حل × هر زیرمسئله) و زمان بدترین تست راه‌حل اصلی؛ باید ۲ تا ۳ برابر کمتر از TL باشد.
   - مراحل آپلود در پنل ادمین، قدم‌به‌قدم، مخصوص همین سوال.
   - جدول ارسال‌های آزمایشی بعد از آپلود: کدام فایل راه‌حل چه امتیازی باید بگیرد.
   - هر چیزی که تأیید نکردی.
```

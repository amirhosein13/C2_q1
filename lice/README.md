# lice — پرش شپش

منبع: Petrozavodsk Summer 2025، روز ۶ (Xeppelin)، مسئله‌ی L «Lice Hopping».
نوع: **Batch با grader** (تابع `int min_days(int N, std::vector<int> U, std::vector<int> V)`، $T$ بار صدا زده می‌شود). زبان: C++.

## حل (model.cpp)
- جواب ≤ ۳ (مکعب هر درخت Hamiltonian-connected است — Sekanina). جواب ۱ ⇔ درخت مسیر است.
- جواب ۲ ⇔ مسیری $P$ (ستون) هست که:
  1. هر شاخه‌ی غیربرگِ آویزان از $P$ «کرم از ریشه» است (هر رأسِ زنجیر حداکثر یک فرزند غیربرگ دارد)؛
  2. رأس‌های $P$ نوع دارند: 0 (همسایه‌ی بیرون از $P$ ندارد)، A (حداکثر یک شاخه‌ی بزرگ)، C (دقیقاً دو شاخه‌ی بزرگ)؛ ≥۳ شاخه‌ی بزرگ ممنوع؛
  3. با حذف Aها، دنباله‌ی نوع‌ها روی $P$ به شکل $0^+(C0^+)^*$ است.
- پیاده‌سازی O(n): goodness یال‌های جهت‌دار با memo روی زنجیر، DP روی درخت ریشه‌دار با ماسک ۳بیتی آتاماتا، و جفت‌کردن دو فرزند در رأسِ بالا با ۱۵ کلاس (O(deg)).
- درستی: لم شاخه‌ها و تحلیل خوشه‌ها اثبات شده؛ «پیمایش ترتیبیِ خوشه‌ها» فقط در حد طرح اثبات است. با brute (بیت‌ماسک روی $T^d$) روی همه‌ی درخت‌های برچسب‌دار n ≤ 8 و حدود ۴۰ هزار درخت تصادفی/هدفمند تا n = 17 چک شده (همه یکسان).

## زیرمسئله‌ها

| # | نام TPS | امتیاز | قید | شمول |
|---|---|---|---|---|
| 0 | samples | 0 | نمونه | — |
| 1 | small | 10 | $N\le15$، $\sum N\le300$ | samples |
| 2 | spider | 12 | حداکثر یک رأس با درجه ≥ ۳ | — |
| 3 | n300 | 18 | $N\le300$، $\sum N\le2000$ | small |
| 4 | n2000 | 25 | $N\le2000$، $\sum N\le2000$ | n300 |
| 5 | full | 35 | $\sum N\le10^6$، $T\le2\cdot10^5$ | n2000, spider |

محدودیت‌ها: ۲ ثانیه، ۲۵۶ مگابایت.

## checker و grader
- grader داوری کل ورودی را می‌خواند، `stdin` را می‌بندد، تابع را صدا می‌زند و بعد رشته‌ی مخفی و `OK` و جواب‌ها را چاپ می‌کند (`has_output_secret`).
- checker (testlib): رشته‌ی مخفی (نبودنش = Protocol Violation با امتیاز ۰) → نتیجه‌ی grader → $T$ عدد در بازه‌ی $[1,3]$ با مقایسه‌ی دقیق؛ خروجی اضافه/ناقص/غیرعددی = WA. جواب یکتاست و گواهی ندارد، پس جوابِ «کمتر از داور» هم WA است.
- `checker/tests/run.sh`: ۱۹ خروجی دست‌ساز با حکم مورد انتظار (همه پاس).

## نتیجه‌ی `tps invoke` (روی این ماشین)

| راه‌حل | samples | small (10) | spider (12) | n300 (18) | n2000 (25) | full (35) | جمع | مطابق انتظار |
|---|---|---|---|---|---|---|---|---|
| model.cpp | AC | AC | AC | AC | AC | AC | 100 | ✓ |
| alt.cpp (O(n) مستقل، rerooting) | AC | AC | AC | AC | AC | AC | 100 | ✓ |
| slow_n2.cpp (O(n²)) | AC | AC | TLE | AC | AC | TLE | 53 | ✓ |
| naive_pairs.cpp (O(Σdeg²)) | AC | AC | TLE | AC | AC | TLE | 53 | ✓ |
| slow_n3.cpp (O(n³)) | AC | AC | TLE | AC | TLE | TLE | 28 | ✓ |
| brute.cpp (بیت‌ماسک) | AC | AC | WA | WA | WA | WA | 10 | ✓ |
| spider.cpp | AC | WA | AC | WA | WA | WA | 12 | ✓ |
| wrong_cc.cpp (دو C بدون 0 بینشان) | AC | WA | AC | WA | WA | WA | 12 | ✓ |
| wrong_nogood.cpp (بدون چک کرم بودن شاخه) | AC | WA | WA | WA | WA | WA | 0 | ✓ |
| wrong_le2.cpp (همیشه ≤ ۲) | AC | WA | WA | WA | WA | WA | 0 | ✓ |
| wrong_caterpillar.cpp (۲ ⇔ کرم) | WA | WA | WA | WA | WA | WA | 0 | ✓ |

زمان‌ها: model حداکثر 0.50s (۴ برابر کمتر از TL)؛ alt حداکثر 1.67s؛ slow_n2 در n2000 حداکثر 0.29s؛ راه‌حل‌های کند در زیرمسئله‌های بزرگ به سقف 4s (hard limit) می‌رسند.
`tps stress`: brute (۱۵۰ دور، n ≤ 15)، alt (۱۵۰ دور، n تا 10⁵)، slow_n2 (۱۰۰ دور): بدون hack.

**زمان‌بندی نهایی باید روی خود سرور CMS دوباره چک شود.**

## import در CMS
```bash
tps export CMS 1 -o lice-cms -a zip
unzip lice-cms.zip && mkdir -p lice/statements && cp statement/lice-fa.pdf lice/statements/fa.pdf
cmsImportTask -L tps_task lice -c <contest_id>
```

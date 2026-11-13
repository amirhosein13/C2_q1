# C2_q1 — سوال‌های آزمون هفتگی دوره‌ی طلا (TPS)

## ساختار

| مسیر | توضیح |
|---|---|
| `tps/` | git submodule از [ioi-2017/tps](https://github.com/ioi-2017/tps) روی commit `fd2b587` (اسکریپت‌ها 1.25) |
| `common/testlib.h` | نسخه‌ی TPS از testlib (سازگار با CMS)؛ در `gen/`، `validator/` و `checker/` هر سوال symlink شده |
| `<task>/scripts` | symlink به `../tps/scripts` |
| `tools/statement/` | ساخت PDF فارسی راست‌به‌چپ از `statement/index.md` (markdown-it + KaTeX + Vazirmatn + Chromium) |
| `tools/cms_loader_dryrun.py` | اجرای loader واقعی `tps_task` در CMS روی خروجی export، بدون پایگاه داده |
| `lice/` | سوال «پرش شپش» |

## راه‌اندازی

```bash
git submodule update --init
(cd tps && sudo ./install-tps.sh)          # فرمان tps
pip install psutil && sudo apt install dos2unix
(cd tools/statement && npm install)        # فقط برای ساخت PDF متن
```

## کار با یک سوال

```bash
cd lice
tps gen                 # تولید و اعتبارسنجی تست‌ها + خروجی‌ها با model solution
tps invoke solution/model.cpp
tps verify
checker/tests/run.sh    # تست checker روی خروجی‌های دست‌ساز
node ../tools/statement/build.js statement/index.md statement/lice-fa.pdf
```

## خروجی و import در CMS

CMS اصلی (cms-dev/cms، loader `tps_task`) فقط **protocol 1** را می‌فهمد
(`task_type_params` را به شکل رشته‌ی JSON می‌خواند).

```bash
cd lice
tps export CMS 1 -o lice-cms -a zip
# exporter متن سوال را export نمی‌کند؛ PDF را دستی اضافه کنید:
unzip lice-cms.zip && mkdir -p lice/statements && cp statement/lice-fa.pdf lice/statements/fa.pdf
# روی سرور CMS:
cmsImportTask -L tps_task lice -c <contest_id>      # به‌روزرسانی: -u
```

بعد از import در AdminWebServer:
- زبان متن (`fa`) را به‌عنوان primary statement انتخاب کنید (loader فقط `en_US` را خودکار primary می‌کند).
- loader این‌ها را ثابت می‌گذارد: `max_submission_number=50`، `min_submission_interval=60s`، score mode = max_subtask؛ در صورت نیاز تغییر دهید.

#!/bin/bash
# Runs the checker on hand-made outputs (for tests/0-01) and compares with expected verdicts.
# Checker is called as CMS/TPS do: <input> <judge answer> <contestant output>.
# Expected: OK = score 1; WA / PV = score 0 (PV shows "Protocol Violation").
cd "$(dirname "$0")"
fail=0
while read -r name exp; do
	../checker.exe ../../tests/0-01.in answer.txt "$name.out" > /tmp/chk_out 2> /tmp/chk_err
	code=$?
	score=$(head -1 /tmp/chk_out); verdict=$(head -1 /tmp/chk_err); msg=$(sed -n 2p /tmp/chk_err)
	case "$exp" in OK) want=1 ;; *) want=0 ;; esac
	ok=yes; [ "$score" == "$want" ] || ok=NO; [ "$code" == 0 ] || ok=NO
	[ "$exp" == PV ] && ! grep -q "Protocol" <<< "$verdict" && ok=NO
	[ "$ok" == yes ] || fail=1
	printf "%-24s exp=%-3s score=%-2s exit=%s  %-28s %s  [%s]\n" "$name" "$exp" "$score" "$code" "$verdict" "$msg" "$ok"
done < expected.txt
exit $fail

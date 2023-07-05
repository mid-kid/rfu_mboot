#!/bin/sh
total="$(grep '\.o(\.text)' demo.ld | wc -l)"
baserom="$(grep 'dump/[^/]*\.o(\.text)' demo.ld | wc -l)"
done="$(expr $total - $baserom)"

percent="$(expr $(expr $done \* 10000) / $total)"
printf '%d.%02d%% (%d/%d)\n' "$(expr $percent / 100)" "$(expr $percent % 100)" "$done" "$total"

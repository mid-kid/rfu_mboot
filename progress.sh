#!/bin/sh
total="$(grep '\.o(\.text)' demo.ld | wc -l)"
baserom="$(grep 'dump/baserom_.*\.o(\.text)' demo.ld | wc -l)"
done="$(expr $total - $baserom)"

percent="$(expr $(expr $done \* 10000) / $total)"
printf '%d.%02d%%\n' "$(expr $percent / 100)" "$(expr $percent % 100)"

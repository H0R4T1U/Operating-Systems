who|awk '{print $1}'|sort|uniq|while read u; do awk -F: '{print $1, $5}' /etc/passwd|grep -E "^$u "|awk '{print $2}' | while read n; do echo $n: `grep -E ":$n " /etc/passwd|awk -F: '{print $1}'`; done; done 
set -x
{ time ./10k_parfasttext.sh ; } &> parfasttext_sh_10k_cerr.txt
{ time ./SW_parfasttext.sh ; } &> parfasttext_sh_SW_cerr.txt
{ time ./TL_parfasttext.sh ; } &> parfasttext_sh_TL_cerr.txt
{ time ./100k_parfasttext.sh ; } &> parfasttext_sh_100k_cerr.txt
{ time ./Full_parfasttext.sh ; } &> parfasttext_sh_Full_cerr.txt
set +x
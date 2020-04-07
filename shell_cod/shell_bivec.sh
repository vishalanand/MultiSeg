set -x
{ time ./10k_bivec.sh ; } &> bivec_sh_10k_cerr.txt
{ time ./SW_bivec.sh ; } &> bivec_sh_SW_cerr.txt
{ time ./TL_bivec.sh ; } &> bivec_sh_TL_cerr.txt
{ time ./100k_bivec.sh ; } &> bivec_sh_100k_cerr.txt
{ time ./Full_bivec.sh ; } &> bivec_sh_Full_cerr.txt
set +x
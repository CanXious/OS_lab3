cmd_/home/ubuntu/OS_lab3/3/3_1/Module.symvers := sed 's/\.ko$$/\.o/' /home/ubuntu/OS_lab3/3/3_1/modules.order | scripts/mod/modpost -m -a -E -o /home/ubuntu/OS_lab3/3/3_1/Module.symvers -e    -w -T -

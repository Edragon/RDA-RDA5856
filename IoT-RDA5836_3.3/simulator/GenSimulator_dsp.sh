#!/bin/bash
if [ "a$1" == "a" ]; then
echo Use Project Name as parameter.
exit 1
fi

echo Generate Project for Project $1
rm -f Simulator/Simulator.dsp 
rm -f Simulator/IncOption.txt
#svn revert CoolSimulator.dsp
cp -f Simulator/Simulator_module.dsp Simulator/Simulator.dsp
#svn revert IncOption.txt
cp -f Simulator/IncOption_module.txt Simulator/IncOption.txt

sed "s/project_name/${1}/g" Simulator/IncOption.txt > tempinc.txt
sed "s/project_name/${1}/g" Simulator/Simulator.dsp > temp.dsp
sed "s/$//" tempinc.txt > Simulator/IncOption.txt
sed "s/$//" temp.dsp > Simulator/Simulator.dsp
rm -f tempinc.txt
rm -f temp.dsp
echo simulator project generated

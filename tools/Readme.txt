
sudo apt-get install tree cflow graphviz 
wget -c https://github.com/tinyclub/linux-0.11-lab/raw/master/tools/tree2dotx
# wget -c https://github.com/tinyclub/linux-0.11-lab/raw/master/tools/callgraph  

# Generate the connections of all directories and files in graph 
tree -L 2 | tree2dotx | dot -Tpng -o kmain.png  

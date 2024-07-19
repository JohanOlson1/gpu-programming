#

## openmp
Once supported internal threading but it is no longer available/recommended! <br>
Use your own theading <bt>

## nvidia-smi
Tool to find information about the GPU
-q (list all info)

## nvtop
A tool to monitor status of a NVIDIA GPU <br>

## Update and switch gcc
> sudo apt install gcc-13 g++-13 -y <br>
> sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 13 --slave /usr/bin/g++ g++ /usr/bin/g++-13 <br>
> sudo update-alternatives --config gcc <br>

## using the GPU with openMP
A lot is needed to get it to work. <br>
Different compilers and GPUs interact differently. Here is the explaination for NVIDA GPU with gcc, used in openMP.  <br>

1. openMP version atleast ? <br>
2. Compile with the following flags: <br>
> -foffload=nvptx-none -fcf-protection=none <br>
The first flag specifies that we want to use a NVIDIA GPU, and the second disables safeguards that prevent offloading. <br>
3. The gcc-13 compiler cannot be used, due to a bug with a dependency to openMP. <br>
4. You need to install the following extension to gcc in order for the compiler to find the GPU: <br>
> sudo apt install gcc-12-offload-nvptx <br>
5. The following directive is used on a for loop to offload to the GPU: <br>
> #pragma omp target teams distribute parallel for map(to:in_data[0:N_elements_], in_data2[0:N_elements_]) map(from:out_data[0:N_elements_]) <br>
The map specifies which data is transferred to and from the GPU. <br>
"to" is used to send data to the GPU before calculations start. <br>
"from" is data that we fetch from the GPU and returns to the CPU memory, hence the result is stored in out_data. <br>
The syntax specifies where the data start and how many elements are fetched. <br>
6. error in compilation TBD, the documentation is frankly atrocious... <br>
> /usr/bin/ld: /tmp/ccCstCfV.crtoffloadtable.o: warning: relocation against `__offload_funcs_end' in read-only section `.rodata' <br>
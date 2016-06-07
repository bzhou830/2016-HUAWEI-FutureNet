一、使用库声明
    项目中使用lp_solve开源库，使用lp_solve源码地址：
https://sourceforge.net/projects/lpsolve/files/lpsolve/5.5.2.0/lp_solve_5.5.2.0_source.tar.gz/download
修改源码中的shell ccc，使用g++ 4.8编译将源码编译成静态库文件。
项目中开源库头文件如下：
1、lp_Hash.h
2、lp_lib.h
3、lp_matrix.h
4、lp_mipbb.h
5、lp_SOS.h
6、lp_types.h
7、lp_utils.h
编译生成静态库文件如下：
liblpsolve55.a
库说明: lp_solve is a free (see LGPL for the GNU lesser general public license) linear (integer) programming solver based on the revised simplex method and the Branch-and-bound method for the integers.
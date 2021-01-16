# OSFES
Operatings Systems for Embedded Systems

## Spherical photo to cubemap

### SPHERICAL
<img src="https://github.com/witek117/OSFES/blob/master/img/original.jpg">

### Cubemap
<table>
  <tr>
    <td><img src="./img/blank.jpg" width = 150px height = 150px style="display: none;"></td>
    <td><img src="./img/blank.jpg" width = 150px height = 150px></td>
    <td><img src="./img/original/PY.jpg" alt="PY" width = 150px height = 150px></td>
    <td><img src="./img/blank.jpg" width = 150px height = 100px></td>
   </tr> 
   
  <tr>
    <td><img src="./img/original/NZ.jpg"  alt="NZ" width = 150px height = 150px ></td>
    <td><img src="./img/original/NX.jpg" alt="NX" width = 150px height = 150px></td>
    <td><img src="./img/original/PZ.jpg" alt="PZ" width = 150px height = 150px></td>
    <td><img src="./img/original/PX.jpg" alt="PX" width = 150px height = 150px></td>
   </tr> 
   
   <tr>
    <td><img src="./img/blank.jpg" width = 0px height = 0px style="display: none;"></td>
    <td><img src="./img/blank.jpg" width = 150px height = 150px></td>
    <td><img src="./img/original/NY.jpg" alt="NY" width = 150px height = 150px></td>
    <td><img src="./img/blank.jpg" width = 150px height = 150px></td>
   </tr> 
</table>


## Build

Run cmake (on Windows)
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - MinGW Makefiles" ..
```

Build
```
make OSFES
```

## Run

```
OSFES.exe -s <source file path> -o <output directory path> -h
```


OSFES.exe -h
```
Spherical photo to cubemap
Usage:
  OSFES [OPTION...]

  -s, --source arg  Param source image (default:
                    .\img\original.jpg)
  -o, --output arg  Param output directory path (default:
                    .\img\original)
  -h, --help        Print usage
```

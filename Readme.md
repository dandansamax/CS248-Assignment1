# CS248-Assignment 3

## Step to run

1. Put project directory in `openFrameworks/apps/`
2. In file `config.make`, change `OF_ROOT` to your own openframework directory.
3. `make`
4. `bin/Assignment1`

## Used libraries

Use [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) only for .obj file loading.

## Optional functionality

- [x] Zooming
- [ ] Triangle clipping
- [x] Spherical coordinate texture mapping
- [x] 3D marble texture
- [ ] Procedural displacement
- [ ] Wireframe model rendering

## Operation
| Key                    | Operation                                          |
| ---------------------- | -------------------------------------------------- |
| P                      | Switch between perspective and parallel projection |
| R                      | Switch between Phong shader and Gouraud shader     |
| Mouse wheel            | Zoom camera                                        |
| Mouse left button drag | Arcball control                                    |

## Enviroment

Hardware: Intel Xeon Gold 6230 CPU @ 2.10GHz, 20 cores
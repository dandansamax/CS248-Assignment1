# CS248-Assignment 2

## Step to run

1. Put project directory in `openFrameworks/apps/`
2. In file `config.make`, change `OF_ROOT` to your own openframework directory.
3. `make`
4. `bin/Assignment1`

## Optional functionality

- [ ] visual controls
- [x] camera zooming / dolly
- [ ] dynamic scene
- [x] modify the position of the light source

## Operation
| Key                      | Operation                                                      |
| ------------------------ | -------------------------------------------------------------- |
| P                        | Switch between perspective and parallel projection             |
| Q                        | To Lambertian shading                                          |
| E                        | To Phong shading                                               |
| A/W/D/S                  | Pan camera left/up/right/down **or** Move light along x/y axis |
| N/M                      | Dolly camera font/back **or** Move light along z axis          |
| Mouse wheel              | Zoom camera                                                    |
| /                        | Switch movement target among camera and all light sources      |
| F/T/H/G                  | Orbit camera left/up/right/down                                |
| Left mouse click         | Select object                                                  |
| left/up/right/down arrow | Translate selected object left/up/right/down                   |
| J/K/L/S-J/S-K/S-L        | Rotate selected object along x/y/z axis                        |
| +/-                      | Scale selected object bigger/smaller                           |

## Enviroment

Hardware: Intel Xeon Gold 6230 CPU @ 2.10GHz, 20 cores
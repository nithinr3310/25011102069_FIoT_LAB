# CAT8 project file status

`Core/Inc`, `Core/Src`, `Drivers/`, and `MDK-ARM/` are scaffold directories only.
They do not yet contain a generated STM32CubeMX/Keil project.

Do not treat this scaffold as a buildable firmware project. Once the board and pin mapping are confirmed, generate the actual project in STM32CubeMX with the Keil MDK-ARM toolchain, then place the generated `Core/`, `Drivers/`, `MDK-ARM/`, `.ioc`, and project metadata here.

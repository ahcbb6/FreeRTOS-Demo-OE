# FreeRTOS-Demo-OE

FreeRTOS demo application for OpenEmbedded's QEMU `virt` machines:

| Machine | CPU | Execution mode | Kernel port |
| --- | --- | --- | --- |
| `qemuarm` | Cortex-A15 | AArch32, privileged modes | `ARM_CA9` |
| `qemuarm64` | Cortex-A57 | AArch64, EL1 | `ARM_AARCH64` |

Both use a single core, GICv2, the architectural physical timer, and a PL011
serial console.

The application echoes printable ASCII input and prints
`Periodic task 10 secs` every ten seconds. Carriage return and line feed
(Enter) each produce `You entered: ""`, followed by `Unblocked` and the ready
message. Other non-printable bytes are ignored so they cannot alter terminal
output.

## Building with Yocto

Use the [meta-freertos layer](https://github.com/ahcbb6/meta-freertos) with
OE-Core and BitBake. In your build's `conf/local.conf`, select:

```bitbake
DISTRO = "freertos"
MACHINE = "qemuarm64"
```

Use `MACHINE = "qemuarm"` instead for 32-bit ARM.

Then build and run:

```sh
bitbake freertos-demo
runqemu nographic
```

The layer fetches this application,
[FreeRTOS-Demo-OE-BSP](https://github.com/ahcbb6/FreeRTOS-Demo-OE-BSP), and
[FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) separately.
Its `freertos-qemuvirt` class supplies Yocto's target compiler and build
flags, passes source paths through `BSP_SRC` and `FREERTOS_SRC`, and
maintains kernel compatibility patches. No BSP, kernel sources, or host
cross-compiler are bundled here.

The build produces `image.elf` and `image.bin`; the layer installs and deploys
them with a QEMU boot configuration that boots the ELF entry point.

## Code organization

- [main.c](main.c): shared tasks, task creation, and application hooks.
- [FreeRTOSConfig.h](FreeRTOSConfig.h): application scheduling, heap, and
  hook policy; includes the BSP's `FreeRTOSBSPConfig.h` for hardware settings.
- [Makefile](Makefile): lists application objects and includes the BSP's
  `freertos-bsp.mk` build rules.

UART/GICv2 drivers, timer scheduling, startup/vectors, linker layout, and
platform diagnostics live in the separate BSP repository.

The layer passes `ARCH=arm` or `ARCH=aarch64` to the same Makefile, selecting
the appropriate kernel port. Yocto uses separate work directories for each
machine; when switching architectures manually in one source tree, run
`make clean` first.

## Runtime tests

Add these settings to `conf/local.conf`, then run
`bitbake freertos-demo -c testimage`:

```bitbake
IMAGE_CLASSES += "testimage"
IMAGE_FEATURES += "allow-empty-password empty-root-password allow-root-login"
TEST_SUITES = "freertos_echo freertos_periodic"
```

## License
MIT; see [LICENSE](LICENSE). The BSP and FreeRTOS-Kernel have their own licenses.

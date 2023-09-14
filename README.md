# CE2727A simple driver

[![Component Registry](https://components.espressif.com/components/latonita/ce2727a-driver/badge.svg)](https://components.espressif.com/components/latonita/ce2727a-driver) [![Example build](https://github.com/latonita/ce2727a-driver/actions/workflows/build_example.yml/badge.svg)](https://github.com/latonita/ce2727a-driver/actions/workflows/build_example.yml)

This repository contains an ESP-IDF driver for a CE2727A power meter connected over UART (RS485).

## Using the component

Run the following command in your ESP-IDF project to install this component:
```bash
idf.py add-dependency "latonita/ce2727a-driver"
```

## Example

To run the provided example, create it as follows:

```bash
idf.py create-project-from-example "latonita/ce2727a-driver:ce2727a-example"
```

Then build as usual:
```bash
cd ce2727a-example
idf.py build
```

And flash it to the board:
```bash
idf.py -p PORT flash monitor
```

....todo....

## License

This component is provided under Apache 2.0 license, see [LICENSE](LICENSE.md) file for details.

## Contributing

Please check [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines.

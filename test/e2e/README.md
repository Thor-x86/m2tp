# End-to-End Testing

This test is similar as simulating a network with M2TP inside your machine with defined test jobs.

> NOTE: Currently, only works with POSIX OS such as Linux and MacOS. Windows won't run the test because of its [exotic API](https://stackoverflow.com/a/54380769/3730834).

To run E2E test, enter this command at project root directory:

```bash
./project build    # Recompile the source code
./project install  # Put the shared object (.so) to your system
./project test e2e
```

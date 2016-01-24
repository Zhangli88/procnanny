# Procnanny
## Description
Procnanny is a process monitoring and cleaning program. It monitors the specified processes in the user's space for their execution for a specified amount of real time, and if any of them exceeds that period, sends them a ```SIGKILL```. The list of processes in the user's space is retreived using the unix command ```ps -u```. If the monitored process dies on its own before specified time, procnanny does nothing.

## Config File
The config file should be a plain text file containing the name of the processes to be monitored along with the monitoring duration in seconds. For example:

```
proca 600
procb 1800
./fooproc 60
```
Note that the process names in the config file should be EXACTLY identical to their names in the ```COMMAND``` column when ```ps -u``` is run. Partially matching names will not be monitored. For example, if the process to be monitored is "proca", ensure the config lists "proca", and not "./proca" or a variant, and vice versa.

## Compiling
The project uses GNU Make, so just compile like this:
```sh
$ make

# For cleaning
$ make clean
```
An "procnanny" executable would be put in the project root directory.


## Running
1. Ensure that the environment variable ```PROCNANNYLOGS``` is defined. This variable should point to a log file that will
be used by procnanny. All the logs will be appended at the end of the file.
2. Start procnanny like this:
    ```sh
    $ ./procnanny /path/to/configfile.txt
    ```
## Usage notes
* The processes to be monitored need not be running when the procnanny first starts. If procnanny notices that a process listed in the config started running, it will start monitoring it then. Similarly, if a process was already running when procnanny started running, it will only monitor it for the specified time starting that instant.
* It is possible to change the config file after starting procnanny. Modify the config file and send a ```SIGHUP``` signal to it. The config file will be re-read, and the new config will be applied to all the processes that will be monitored starting then. The processes that were already being monitored by procnanny will continue to be monitored as per the old config.
* Send a ```SIGINT``` to procnanny to safely kill it.
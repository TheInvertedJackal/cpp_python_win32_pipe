# Ideas for Upgrades
## Config File
Add a way to connect the two programs and control things like pipe count etc.
- C++ to Python Pipe Count
- Python to C++ Pipe Count
- C++ Buffer Size
- Python Buffer Size
- Comments

## Multi-Threaded Output(s)
When posting data, you post it on a different thread, each thread has it's own pipe.
- Have load balancing so each pipe takes roughly the same number of messages

## More than one Thread Input
When importing data, get it from multiple threads, but place it in the same place.
- Add a C++ message box class like Python (Roughly the Same Logic)

## Add Utilities (Seprate from the rest of the )
Add ways to quickly pull specific data from the byte string
- Byte to Double
- Double to Byte
- Int to Byte
- Byte to Int

## Clean Up
- Change the direct name space naming to using
- Add example usage files
- Remove Testing Files (and find a better way to test)
# Simple Formatting

Currently, there are no text formatting procedures that have advanced options such as padding, digit precision, etc.
While I plan the design of more advanced formatting procedures, simple procedures can be used insead.

## Functions and Macros

There is a function for general formatting and output called ```simple_printf```  and a macro for logging called ```simple_log```.

### ```simple_printf```

To output and format text, ```simple_printf``` can be used.

The formatting uses C-like syntax to determine where to place arguments.
For example, the following statement formats an integer into a string before output:
```c
#include <std/log.h>

simple_printf("Integer: %i", 1234);     // => "Integer: 1234"
```

There are no advanced formatting options such as padding or digit procedures.
Only the type of argument to be formatted is needed.

#### Syntax

The following rules are used to format text:
```c
#include <std/log.h>

simple_printf("Integer: %i", 1234);         // => "Integer: 1234"
simple_printf("Integer: %d", 1234);         // => "Integer: 1234"
simple_printf("Hex: %x", 0xdeadbeef);       // => "Hex: 0xdeadbeef"
simple_printf("String: %s", "kernel");      // => "String: kernel"
simple_printf("Character: %c", 'k');        // => "Character: k"
simple_printf("Pointer: %p", 0xdeadbeef);   // => "Pointer: 0x00000000deadbeef"
simple_printf("Bits: %b", 7);               // => "Bits: 0b0000 0000 0000 0000 0000 0000 0000 0111"
```

### ```simple_log```

The macro ```simple_log``` uses the same formatting rules as ```simple_printf```, but adds some extra details.

Each call to ```simple_log``` prints a new line character at the end.
At the start of each line is the file and line that the log call originated from.
This allows for easier debugging compared to using ```simple_printf```.

Thus, ```simple_printf``` should be used for normal output and ```simple_log``` should be used for debug output.

#### Example

The following is a basic example of a ```simple_log``` call and its output:
```c
simple_log("Hello %s", "kernel!");  // => [boot/arch/x86_64/main.c:129] Hello kernel!
```

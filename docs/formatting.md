# Formatting

NOTE: Currently, the macros described in this document are not implemented.
See [Simple Formatting](./simple_formatting.md) for a description of the currently implemented output macros.

Formatting rules for printing macros ```kprint```, ```kerror```, ```kpanic```, and ```klog```.

These macros allow for outputing text at any place in the kernel.
The formatting rules are based off of Python's f-strings and Rust's ```format!``` macro.

## Purpose

The formatting rules of this kernel's logging library are delibrerately very similar to Rust's ```format!``` macro.
The main reason for deriving these rules off of ```format!``` instead of ```printf``` is because ```format!``` allows for variables to be directly inserted into the formatting brackets.
This is shown in an example below

Both of these examples output the same text.
Even though ```print!``` is used in this example, ```format!``` shares the same grammer rules.
```c
const char *kernel = "kernel";
printf("Hello %s", kernel);     // => Hello kernel
```

```Rust
const kernel: &str = "kernel";
print!("Hello {kernel}");       // => Hello kernel
```

## Usage

The formatting properties of these macros are a trimmed down version of the Rust ```format!``` macro.
They generally have the same grammer rules, but a few features are left out (such as named parameters).

### Examples

These examples are similar to Rust's [format!](https://doc.Rust-lang.org/std/fmt/index.html) macro.
They use the ```kprint``` macro, which prints the formatted string to ```stdout``` (stdout can currently be any function pointer with specific parameters; the default is UEFI text output).

```c
kprint("Hello kernel!");                    // => Hello kernel
kprint("Hello {}!", "kernel");              // => Hello kernel
kprint("The number is {}", 1);              // => The number is 1
const char *kernel = "always sunny";
kprint("Hello {kernel}!");                  // => Hello always sunny
kprint("{} {}", 1, 2);                      // => 1 2
kprint("{:04}", 42);                        // => 0042
kprint("{:04x}", 0xff);                     // => 0x00ff
```

The first parameter is the format string where variables and parameters are formatted into.
This parameter needs to be a string literal; it cannot be a variable.
However, it could be another macro that inserts a string literal at compile time.

## Positional Parameters

The easiest way to format a value is to supply it as a positional parameter.
Each parameter matches to the set of braces in the same position of the format string.
In the following example, the first set of braces is replaced by the first parameter, the second set of braces is replaced by the second parameter, and so on.

```c
// => Hello always sunny operating system
kprint("Hello {} {} {} {}", "always", "sunny", "operating", "system");
```

## Syntax

This set of the full grammer rules have been taken from Rust's ```format!``` macro.
```
format_string := text [ maybe_format text ] *
maybe_format := '{' '{' | '}' '}' | format
format := '{' [ argument ] [ ':' format_spec ] '}'
argument := int | identifier

format_spec := [[fill]align][sign]['#']['0'][width]['.' precision]type
fill := char
align := '<' | '^' | '>'
sign := '+' | '-'
width := count
precision := count | '*'
type := '' | '?' | 'x' | 'X' | 'x?' | 'X?' | 'o' | 'p' | 'b' | 'e' | 'E'
count := parameter | int
parameter := argument '$'
```

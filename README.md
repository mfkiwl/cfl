cfl
===================================

Author: Anthony Burzillo

******

cfl (standing for C-based Functional Language) is a statically typed
functional programming language.

## Usage

To see the usage simply run `./cfl`. If LLVM is already installed (can be installed by running
`brew install llvm` or `apt-get install llvm`, etc.) then the usage will be
```
$ ./cfl
USAGE: cfl filename       :: compile the program
           -ast filename  :: print the AST of the program
           -type filename :: print the high level types of the program
           -deep filename :: print all the types of the program
           -eval filename :: evaluate the program
           -ll filename   :: print out the llvm code for the program
           -asm filename  :: print out native assembly code for the program
           -jit filename  :: evaluate the program using Just-In-Time compiling
```
Note that you can still run cfl without LLVM but you will not have LLVM related options like
-jit or compilation.

## Example

Obligatory hello world:
```
main = "hello world!"
```
Note that every cfl program must end with a `main` and global definitions are followed by
a semicolon.

The quicksort algorithm can be written in cfl as
```
split x xs l r = case xs of
      []       -> (l, r)
    | (y : ys) -> let (l', r') = split x ys l r in if y <= x
        then (y : l', r')
        else (l', y : r');

quicksort x = case x of
      []       -> []
    | (y : ys) -> let (l, r) = split y ys [] []
        in quicksort l ++ y : quicksort r;

// random x is a globally defined function returning a random integer
// between 0 and x inclusive
random_list x l = if x == 0 then [] else random l : random_list (x - 1) l;

main = quicksort $ random_list 100 500
```

Running `./cfl -ast` will output
```
split ::= function ~F0 -> (let rec split x = (function xs -> (function l -> (function r -> (case (xs) of [] -> ((l, r)) | (y : ys) -> ((function (l', r') -> (if (((y) < (x)) || ((y) == (x))) then (((y) : (l'), r')) else ((l', (y) : (r'))))) (((((split) (x)) (ys)) (l)) (r))))))) in ((split) (~F0)))
quicksort ::= function ~F1 -> (let rec quicksort x = (case (x) of [] -> ([]) | (y : ys) -> ((function (l, r) -> (((quicksort) (l)) ++ ((y) : ((quicksort) (r))))) (((((split) (y)) (ys)) ([])) ([])))) in ((quicksort) (~F1)))
random_list ::= function ~F2 -> (let rec random_list x = (function l -> (if ((x) == (0)) then ([]) else (((random) (l)) : (((random_list) ((x) + ((-1) * (1)))) (l))))) in ((random_list) (~F2)))
main ::= (quicksort) (((random_list) (100)) (500))
```

Running `./cfl -type` will output
```
split => (Integer -> ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer])))))
quicksort => ([Integer] -> [Integer])
random_list => (Integer -> (Integer -> [Integer]))
main => [Integer]
```

Running `./cfl -deep` will output
```
split ~>> function (~F0 :: Integer) -> (let rec (split :: (Integer -> ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer])))))) (x :: Integer) = (function (xs :: [Integer]) -> (function (l :: [Integer]) -> (function (r :: [Integer]) -> (case (xs :: [Integer]) of [] -> ((l :: [Integer], r :: [Integer]) :: ([Integer], [Integer])) | ((y :: Integer) : (ys :: [Integer])) -> ((function ((l' :: [Integer], r' :: [Integer]) :: ([Integer], [Integer])) -> (if (((y :: Integer) < (x :: Integer) :: Bool) || ((y :: Integer) == (x :: Integer) :: Bool) :: Bool) then (((y :: Integer) : (l' :: [Integer]) :: [Integer], r' :: [Integer]) :: ([Integer], [Integer])) else ((l' :: [Integer], (y :: Integer) : (r' :: [Integer]) :: [Integer]) :: ([Integer], [Integer])) :: ([Integer], [Integer])) :: (([Integer], [Integer]) -> ([Integer], [Integer]))) (((((split :: (Integer -> ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer])))))) (x :: Integer) :: ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer]))))) (ys :: [Integer]) :: ([Integer] -> ([Integer] -> ([Integer], [Integer])))) (l :: [Integer]) :: ([Integer] -> ([Integer], [Integer]))) (r :: [Integer]) :: ([Integer], [Integer])) :: ([Integer], [Integer])) :: ([Integer], [Integer])) :: ([Integer] -> ([Integer], [Integer]))) :: ([Integer] -> ([Integer] -> ([Integer], [Integer])))) :: ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer]))))) in ((split :: (Integer -> ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer])))))) (~F0 :: Integer) :: ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer]))))) :: ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer]))))) :: (Integer -> ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer])))))
quicksort ~>> function (~F1 :: [Integer]) -> (let rec (quicksort :: ([Integer] -> [Integer])) (x :: [Integer]) = (case (x :: [Integer]) of [] -> ([] :: [Integer]) | ((y :: Integer) : (ys :: [Integer])) -> ((function ((l :: [Integer], r :: [Integer]) :: ([Integer], [Integer])) -> (((quicksort :: ([Integer] -> [Integer])) (l :: [Integer]) :: [Integer]) ++ ((y :: Integer) : ((quicksort :: ([Integer] -> [Integer])) (r :: [Integer]) :: [Integer]) :: [Integer]) :: [Integer]) :: (([Integer], [Integer]) -> [Integer])) (((((split :: (Integer -> ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer])))))) (y :: Integer) :: ([Integer] -> ([Integer] -> ([Integer] -> ([Integer], [Integer]))))) (ys :: [Integer]) :: ([Integer] -> ([Integer] -> ([Integer], [Integer])))) ([] :: [Integer]) :: ([Integer] -> ([Integer], [Integer]))) ([] :: [Integer]) :: ([Integer], [Integer])) :: [Integer]) :: [Integer]) in ((quicksort :: ([Integer] -> [Integer])) (~F1 :: [Integer]) :: [Integer]) :: [Integer]) :: ([Integer] -> [Integer])
random_list ~>> function (~F2 :: Integer) -> (let rec (random_list :: (Integer -> (Integer -> [Integer]))) (x :: Integer) = (function (l :: Integer) -> (if ((x :: Integer) == (0 :: Integer) :: Bool) then ([] :: [Integer]) else (((random :: (Integer -> Integer)) (l :: Integer) :: Integer) : (((random_list :: (Integer -> (Integer -> [Integer]))) ((x :: Integer) + ((-1 :: Integer) * (1 :: Integer) :: Integer) :: Integer) :: (Integer -> [Integer])) (l :: Integer) :: [Integer]) :: [Integer]) :: [Integer]) :: (Integer -> [Integer])) in ((random_list :: (Integer -> (Integer -> [Integer]))) (~F2 :: Integer) :: (Integer -> [Integer])) :: (Integer -> [Integer])) :: (Integer -> (Integer -> [Integer]))
main ~>> (quicksort :: ([Integer] -> [Integer])) (((random_list :: (Integer -> (Integer -> [Integer]))) (100 :: Integer) :: (Integer -> [Integer])) (500 :: Integer) :: [Integer]) :: [Integer]
```

Running `./cfl -eval` or `./cfl -jit`, or running `./cfl program.cfl` and then `./program` will output something like
```
[0, 1, 3, 8, 14, 15, 16, 17, 25, 32, 37, 38, 40, 46, 61, 61, 64, 77, 80, 106, 108, 110, 111, 113, 121, 127, 134, 134, 137, 144, 149, 151, 158, 160, 165, 177, 179, 180, 188, 192, 196, 200, 201, 202, 212, 216, 218, 220, 223, 230, 236, 239, 239, 244, 251, 253, 265, 269, 281, 294, 301, 304, 308, 315, 323, 332, 334, 356, 359, 372, 373, 374, 375, 375, 383, 385, 387, 392, 393, 397, 398, 399, 402, 412, 420, 425, 431, 433, 438, 440, 441, 445, 452, 454, 459, 474, 482, 488, 489, 495]
```

## Expressions

### Basic types

The basic types of cfl are bools (`true` or `false`), integers (`0, 1, -1, ...`), and
characters (`'q'`).

### Boolean Operations

cfl allows conjunction (`&&`), disjunction (`||`), and negation (`!`) expressions.

### Integer Operations

The operations of addition (`+`), subtraction (`-`), multiplication (`*`), division
(`/`), and modulus (`%`) evaluate to integer values. On the other hand, the comparison
operators of equality (`==`), inequality (`!=`), less-than (`<`), less-than-or-equal
(`<=`), greater-than (`>`), and greater-than-or-equal (`>=`) all evaluate to boolean
values.

### Functional Operations

cfl allows the creation of anonymous functions via `function x -> e` where `e` is some
expression, and `x` is the argument to the function. We can create multiple argument
anonymous functions via `function x -> function y -> e` where `x` is
the first argument and `y` is the second, etc.

To apply a function `f` simply use `f e` where `e` is the expression for the first
argument, or `f e1 e2` for the first argument `e1` and second argument `e2`. Similary
one can use `f $ x` to apply the entire right side of the `$` to `f`.

Finally, one can compose two functions `f . g`.
```
g a = if a then 5 else 4;
f b = b == 5;
main = f . g $ true
```

### If Expressions

Use `if e1 then e2 else e3`.

### Let Expressions

cfl allows let expressions to define constants and functions (possibly recursive) via
```
let x = 4 in x + 54
```
and
```
let f x y = if x = 0 then 0 else y + f (x - 1) y in f 3 4
```

### Tuple Expressions

Tuples of arbitrary size can be created via `(true, false, [0], 1, [false])`.

Anything but a function name can be matched to a tuple in `let` statements, for instance
```
let (x, y) = (1, 2) in x + y
let f (x, y) = x + y in f 0 1
```
are both valid.

### List Expressions

You can create empty lists `[]` or lists with values `[1, 2, 3, 4]`. You can push
values onto a list via
```
5 : [4, 5, 6]
```
You can also concatenate lists
```
[1, 2] ++ [3, 4]
```
Finally, we can match on lists via a case expression via
```
case e1 of [] -> e2 | (x : xs) -> e3
```

### String Expressions

Strings are implemented as lists of characters, therefore characters can be pushed onto
strings and two strings can be concatenated. To create a string one can simply write the
quotation enclosed string, i.e. `"hello world!"`.

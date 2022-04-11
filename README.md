# Learn Czech Gender
I'd like to write a simple commandline program that will help me learn Czech
genders, separate from Anki. Anki is great, but repetition is key here, not just
committing to long-term memory. 

Why write it in C? Shut up, I just wanted to. I need to keep my C skills sharp. 

## Goals
Given a word list in a consistent, simple format, prompt me for the gender of
the word and then correct me when I'm wrong. What about spacing the repetition?
A word should not come up more than once a day. Also, if I do the words in the
same order every day, that would be predictable and unproductive. I could get
prompted with a random word each time, which would guarantee equal and random
representation. For a first pass, this might be good enough, but I might want a
more complicated scheme at some point. 

Hm. That method would also be pretty annoying, since all words come up equally
infrequently. 

Still, it might be good for random practice sessions when I'm not doing Anki.
Idle time could be spent practicing the genders of random words that I _should_
know the genders for already, if Anki did it's job. 

Other requirements: 

- Should handle me updating a list periodically, so it should read its input
  from a file. I'll need some way to indicate the genders of words within the
file. 
- Czech uses more than ASCII characters, so I need to make sure I can handle
  UTF-8, multi-byte characters. This is a new problem for me in C, so more on
this later. 

And that should do it. An example session would be:

```
~ $ gendercz

žena (f/m/n): f
Correct!

nebe (f/m/n): f
Incorrect! nebe shatters. 
^C

Goodbye!
~ $ 
```

For speed, I'll test it first in Python and then rewrite it in C for practice. 

## Data serialization format
I'm not going to to anything unnecessary, just a simple tab-delimited text file.
For each line, the first item is the word, the second item is the gender. I'll
build in the imagery for each. Since I will already be reading in UTF-8
characters, I can continue to do that and write the genders in Czech. Although
it might be easier to write the genders in English for later processing... 


## UTF-8, multi-byte strings
I wrote a minimal program that reads from stdin and prints to stdout, and it
worked with unicode characters. So that's a good thing. Other string matching
functions should also work in the `string` library, according to one person. 


## C style
It's also time for me to learn some threadsafe, modern C. Here's an example
routine for reading utf8 input. 

```c
char *get_utf8_input()
{
    char *line, *u8s;
    unsigned int *wcs;
    int len;

    line = readline("");
    if (local_is_utf8) {
        return line;
    }
}
```

The program goes on a little longer, but this is good enough. `readline()` is a
routine from the GNU Readline library. It allows command line editing, and it is
the library that powers your shell most likely. You can insert a prompt, or if
you pass an empty string (as here) then there is no prompt. 

Let me try to write my own readline style input program, similar to the minimal
program I just wrote. 

**Compilation note:** When you compile, you must include a library argument so
the linker knows which libraries to search. In the case of `readline`, you send
`-lreadline` or `-l readline` as the argument. 

```
gcc main.c -l readline -o gendercz
```

**Side note:** I just learned about the Curses library, which gives you more
control over the terminal screen. This is what you would use if you wanted to
write your own text editor, probably. Also, you can control the TTY behavior
with `stty`. See `man stty` for more information. But you can stop echo (e.g.,
for passwords). 



## Back to the serialization problem
I need a few functions to complete the serialization problem, which shouldn't be
too bad. I just need a refresher on file I/O and text processing. Let's break it
down. 

### Parse the line
This seems like a straightforward task.

- Input: a pointer to a line of text
- Output: The gender or the word, depending on the argument

Every line of text is formatted exactly the same: word, tab, gender character.
Thus, the problem becomes one of getting every _before_ the tab or _after_ the
tab, excluding the tab in both cases. 


The function to find the first occurrence of a character is `strchr()` in
`<string.h>`. 

```c
char *strchr(const char *str, int ch);
```

This returns a pointer to that character, or a NULL pointer if it is not found.
I need a routine that somehow gives me access to the substring. That could mean
copying the word to a new string. 

Okay, so, let's copy the string and then replace the tab with a null string
character. I can use `strncpy`, which copies over a certain amount of a word. 

Maybe it's simpler to copy over one character at a time, until you hit the tab
character. I can still use `strchr` to find the starting point of the gender. 


Okay, so both of those are implemented. You can copy the word to a pre-allocated
array, and you can return the gender of the word as a character. Now I need a
few more pieces to get this running:

- Read input from a file
  - Open the file for reading
  - Copy a line of the file to a string
- Take user input and compare to the correct gender
  - Parse the word and use it in the prompt
  - Compare the first character of the input to the correct gender

It might be a good idea to process the whole file in the beginning and store
everything in an array that I can pass to important functions. So, generate the
database, then query it. This will also help when I need to choose random words
from the dictionary. 

In fact, this is a type of map. Hm. That makes things a little complicated. I
guess it's not such a map if I use a struct and index it with numbers. Actually,
that's probably necessary to be able to return a random element. Another option
is a linked list, which I haven't implemented in a while. That's okay, a struct
is fine. 

Cool, I have a struct that works, and for the time being I'm planning on simply
allocating a bunch of members of it. Now let's work on filling in that struct
using a file. I need to read a line of the file, parse it into a word and a
gender, and then store those in the struct. Note that the word element of the
struct is a pointer, so the word still needs to be allocated somewhere. At the
end of the program I can free the whole array (or whatever parts of it I
actually use). 

That last part's tricky. Behavior of `free` is undefined if the space has never
been allocated.

Okay, for this project, I want to always allocate and deallocate memory in the
same scope. 

Now, back to the problem. I need this struct and its elements while the program
is running. So, the easiest solution is to probably: 

- Set a variable for the number of words in the list
- Allocate each word as it is read
- If a line is corrupt, free all previous structs and break
- Otherwise, free everything by looping over it at the end of the program. 

---

Charlie Gallagher, April 2022

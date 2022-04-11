# Learn Czech Noun Genders
gendercz is a small C program I wrote to help me learn Czech noun genders. 

### Install

```
gcc -o gendercz main.c
```

## Getting Started
The program takes a formatted input text file and uses that to build a
"database" of words and genders, which is then quizzes you on. 

Note that I wrote this in an afternoon after a hiatus from C, so apologies if
it's garbage. 

### Input file format
The format is `word<tab>gender` where `gender` is a single character, one of m,
f, or n at the moment. 

```
Rodina	f
snídaně	f
věc	f
Prase	n
dort	m
Žena	f
pes	m
Kukuřice	f
```

The tab is important, and this input file format is pretty darn fragile. 

### Running the program
It's pretty straightforward: `gendercz <input_file>`. Here is the output from
the above word list. 

```
~ $ gendercz gender_nouns.txt

Rodina (m/f/n): f
Good job!

snídaně (m/f/n): n
Bad job! The correct gender was f.

věc (m/f/n): q
Goodbye!

~ $
```

At the moment, the words are quizzed in order, because I didn't feel like
randomizing it. You can actually randomize your list of words yourself with
`shuf` or `sort -R`. 


---

Charlie Gallagher, April 2022

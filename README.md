<div align="left">
  <h1>Poems-Editor</h1>
  <h3>A program for editing Romanian poems</h3>
</div>
<br/>
<br/>

## How to use ##
Firstly, run the program in an IDE or in the terminal.

The program works based on an interactive console. 

The first command has to be:

`load <path>`

where <path> is the relative path to a test file in the poems folder.
  
Example: `load poems/test1/test1`

Other supported commands:

* uppercase: Makes the first letter of each verse uppercase and the others lowercase
* trimming: deletes multiple spaces in the poem
* make_it_silly P: each letter has the probability P of changing from uppercase to lowercase 
and the other way around; P is a real number between 0 and 1
* make_it_friendlier: replace some words with their diminutives
* make_it_rhyme rhyme_type: makes verses rhyme by last letter based on the rhyme_type;
the rhyme type can be:
  * imperecheata (verses 1-2 and 3-4)
  * incrucisata (verses 1-3 and 2-4)
  * imbratisata (verse 1-4 and 2-3)
* print: prints the current state of the poem
* quit: exits the interactive program

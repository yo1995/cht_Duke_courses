TCP example by Brian Rogers, updated by Rabih Younes. Duke University.
-----------------------------------------------------------------------

Build using make. To test:

In one terminal, run the server. 

In a second terminal, run the client with the hostname of the server as a parameter.
If both server and client are the same machine, use 'localhost' for the client.



Single machine example:
  On terminal 1:
    $ ./server
  On terminal 2:
    $ ./client localhost

Multiple machine example:
  On a machine called 'host1':
    $ ./server
  On another machine:
    $ ./client host1

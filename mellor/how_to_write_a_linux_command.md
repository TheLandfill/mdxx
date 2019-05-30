# How to Make a Linux Command

## Quick Answer

I'm going to go through a step by step example of how I would generally make a Linux command.
For this example, I want the command test-command to run the script /home/joseph/dev/example/hello-command.sh, which should print out some form of success when run.

1.  `chmod 755 ~/dev/example/hello-command.sh`
2.  `ln -s ~/dev/example/hello-command.sh /usr/local/bin/test-command`

Note that `~/` is an abbreviation for `/home/current-user/`.
If you want a more in-depth answer, read on.

## What Exactly is a Linux Command?

If you have any experience with Linux or bash, you'll already know a few standard Linux commands, such as `cd` (change current directory), `mv` (move or rename files and folders), and `cp` (copy files).
If you're a little more experienced, you might know more about some more complex commands, such as `sudo` (which gives a command temporary administrative privileges) or `python` (runs the python interpreter).
Because you really can't do much in your terminal without these commands and they come on most terminals, people often assume that they're built in.
While a few of them are (`cd` usually is), most are not.
They're programs or scripts that are in a specific folder.
In fact, you can use either the `which` command or `type -a` to see exactly what folder a command is in.
For example:
[!A few examples of `which` and `type -a`](which-type-examples.xcf)

As you can see, besides `cd`, the other Linux commands are actually executables in either /bin/ or /usr/bin/.
Any executable in either of these folders can be run from anywhere just like a Linux command.
There are several other directories that also can also have commands, which are contained in the PATH environment variable.
To see the full list, type `echo $PATH` into your terminal.
[!List of directories in my PATH](path-examples.xcf)

You should see a list of directories separated by colons, as shown above.

### Making an Executable

Before you can run the executable, you have to tell the system which users can run it by using the `chmod` command.
The `chmod` command allows you to set who can read, write, and execute a file.
In most cases, you can use `chmod 755 exe`, which will allow anyone to execute exe, but only allow the user to modify it.
For more information, see [here](https://askubuntu.com/questions/932713/what-is-the-difference-between-chmod-x-and-chmod-755) or [here](https://www.thegeekstuff.com/2010/06/chmod-command-examples/).

## Making a Linux Command

Since any executable must be in one of the directories specified in PATH, we now have a few ways to make a Linux command.

1.  Add the directory containing your executable to the PATH variable.
2.  Move the executable into one of the directories currently in PATH.
3.  Create a symbolic link to your executable in one of the directories in PATH.

### Adding a Directory to the PATH Variable

All you have to do is just add the directory to your PATH variable, using the syntax:
```bash
export PATH=$PATH:<directory to add>
```
For example, `export PATH=$PATH:/home/username/dev/` will allow you to call any executable in the folder `/home/username/dev/`.

Unfortunately, you would have to run this command whenever you log back in, as $PATH will get reset to its default value.
It's pretty easy to get around this limitation, as you just have to add the line to the file ~/.bashrc, where '~' is an alias for your home directory.
Your ~/.bashrc file is a list of commands that run when you login to a shell or when you run the command `source` with ~/.bashrc as the argument.
By adding `export PATH=$PATH:<directory to add>` to your ~/.bashrc, the directory will be added to your PATH and you can run any executable within that directory by calling it by its name.

Adding another directory to the path variable could end up masking commands, as each directory in PATH is searched in the order in which it shows up in PATH.
The first command in PATH with the same name as the command you typed in will be executed.

### Moving the Executable

Moving the executable isn't too hard either, but there are a few things to watch out for.
First, you'll need to use `sudo mv` or `sudo cp` most of the time, as these directories are restricted to root access.
Second, you'll need to make sure that the executable doesn't depend on any files that wouldn't also be in the directory.
If you use absolute file paths in the executable, you'll be fine, but you will need to rewrite your code if you use relative file paths.
Lastly, you'll probably want to put your executable in a non-essential directory like `/usr/local/bin/` to avoid destroying fundamental commands like `cp` and `mv`.

With this approach, you may have to copy or move the executable whenever you update it.
You also will end up mixing files your executable depends on with commands, which could complicate things.
You'll either end up bringing in all the files in your project to the directory, which could also be considered executables, or you'll split your project into segments, which could be harder to manage.

### Using Symbolic Links

Luckily, symbolic links have neither of the cons of the other two methods.
Symbolic links allow a file or directory to be accessed from a location other than its actual location.
You can imagine them as files that just tell the operating system.

You can create one using the syntax:
```bash
ln -s <path of file you want to link to/name of file> <path of link/name of link>
```

Since the file isn't actually moved, you're less likely to have name clashes because of two programs in different directories, you don't have to worry about copying it whenever you update it, and it won't split your project in half.
Like filepaths, symlinks can be relative or absolute.
You should use absolute filepaths when making the symlink.
You'll also generally need to use `sudo` as these directories require root access.
If you add your own directory to PATH, you won't need to use sudo.
Lastly, if you move the file the symlink refers to, you will need to delete the symlink using `rm` and link it to the new file.

## Note About Aliases

If you just want to run a simple command or a simple series of commands, you can modify `~/.bashaliases` (or `~/.bashrc` directly if that doesn't work) and just add a line. 
The syntax is `alias <alias name>='command'`.
For example, I have a few aliases related to the `ls` command.
I also wrote another alias that prints a few lines of dashes in a different color and then runs `make` so I can distinguish old errors from new errors.


let ensure = (bool, message) => if (!bool) {
  print_endline("Failed: " ++ message)
};

/* Normal command */
let (lines, succeeded) = Commands.readCommand(~cmd="echo 'hi'", ());
print_endline(String.concat("\n", lines));

/* Pollable command */
let (poll, close) = Commands.pollableCommand(~cmd="yes", ~onOut=line => ());
let t = Unix.gettimeofday();
while (Unix.gettimeofday() -. t < 0.01) {
  poll();
};
close();

/* Keepalive */
let (poll, close) = Commands.keepAlive(~cmd="yes", ~checkTime=0.01, ~onStart=() => print_endline("(re)starting"), ());
let t = Unix.gettimeofday();
while (Unix.gettimeofday() -. t < 0.2) {
  poll();
};
close();

ensure(Files.exists("./"), "directory exists");
ensure(Files.exists("./bsconfig.json"), "file exists");
ensure(!Files.exists("./noexist.json"), "nonexistant file");

ensure(Files.readDirectory("./test") == ["Test.re"], "read dir");

let tmp = "./lib/tmp";
Files.mkdirp(tmp);

let contents = "This is awesome\nand I like it";
let path = Filename.concat(tmp, "awesome.txt");
Files.writeFile(path, contents);
ensure(Files.readFile(path) == Some(contents), "write & read a file");

let (/+) = Filename.concat;
let deep = "a" /+ "b" /+ "c" /+ "more.thing";
let deepContents = "more stuff";
Files.mkdirp(Filename.dirname(tmp /+ deep));
Files.writeFile(tmp /+ deep, deepContents);

let tmpCopy = "tmpCopy";
Files.copyDeep(~source=tmp, ~dest=tmpCopy);

ensure(Files.readFile(tmpCopy /+ deep) == Some(deepContents), "write & read a file");

Files.removeDeep(tmp);
Files.removeDeep(tmpCopy);

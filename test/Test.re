
let tests = ref(0);
let failures = ref(0);

let ensure = (bool, message) => {
  tests := tests^ + 1;
  if (!bool) {
    print_endline("Failed: " ++ message);
    failures := failures^ + 1;
  };
};

let report = () => {
  print_newline();
  if (failures^ == 0) {
    print_endline("Success! All " ++ string_of_int(tests^) ++ " tests passed");
    exit(0);
  } else {
    print_endline("Failures: " ++ string_of_int(failures^) ++ " / " ++ string_of_int(tests^));
    exit(1);
  }
};

/* Normal command */
let (lines, succeeded) = Commands.readCommand(~cmd="echo 'hi'", ());
ensure(lines == ["hi"], "echo outputs hi");
ensure(succeeded, "echo exited successfully");

/* Pollable command */
let (poll, close) = Commands.pollableCommand(~cmd="yes", ~onOut=line => ());
let t = Unix.gettimeofday();
while (Unix.gettimeofday() -. t < 0.01) {
  poll();
};
close();

/* Keepalive */
let starts = ref(0);
let (poll, close) = Commands.keepAlive(~cmd="yes", ~checkInterval=0.01, ~onStart=() => starts := starts^ + 1, ());
let t = Unix.gettimeofday();
while (Unix.gettimeofday() -. t < 0.2) {
  poll();
};
close();
ensure(starts^ == 1, "yes starts only once");

let starts = ref(0);
let (poll, close) = Commands.keepAlive(~cmd="echo 'hi'", ~checkInterval=0.05, ~onStart=() => starts := starts^ + 1, ());
let t = Unix.gettimeofday();
while (Unix.gettimeofday() -. t < 0.2) {
  poll();
};
close();
ensure(starts^ > 1, "echo starts a bunch");

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
ensure(!Files.exists(tmp), "tmp removed");
ensure(!Files.exists(tmpCopy), "tmp copy removed");

report();
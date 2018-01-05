
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
let (lines, succeeded) = Commands.execSync(~cmd="echo 'hi'", ());
ensure(lines == ["hi"], "echo outputs hi");
ensure(succeeded, "echo exited successfully");

ensure(Files.exists("./"), "directory exists");
ensure(Files.exists("./bsconfig.json"), "file exists");
ensure(!Files.exists("./noexist.json"), "nonexistant file");


let tmp = "./lib/tmp";
Files.mkdirp(tmp);

let contents = "This is awesome\nand I like it";
let path = Filename.concat(tmp, "awesome.txt");
Files.writeFile(path, contents);
ensure(Files.readFile(path) == Some(contents), "write & read a file");

ensure(Files.readDirectory(tmp) == ["awesome.txt"], "read dir");

let (/+) = Filename.concat;
let deep = "a" /+ "b" /+ "c" /+ "more.thing";
let deepContents = "more stuff";
Files.mkdirp(Filename.dirname(tmp /+ deep));
Files.writeFile(tmp /+ deep, deepContents);

let tmpCopy = "tmpCopy";
Files.copyDeep(~source=tmp, ~dest=tmpCopy);

ensure(Files.readFile(tmpCopy /+ deep) == Some(deepContents), "write & read a deep file");

Files.removeDeep(tmp);
Files.removeDeep(tmpCopy);
ensure(!Files.exists(tmp), "tmp removed");
ensure(!Files.exists(tmpCopy), "tmp copy removed");

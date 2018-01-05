
let maybeStat = path => try {Some(Unix.stat(path))} {
| Unix.Unix_error(Unix.ENOENT, _, _) => None
};

let copy = (source, dest) => {
  switch maybeStat(source) {
  | None => false
  | Some({Unix.st_perm}) =>
    let fs = Unix.openfile(source, [Unix.O_RDONLY], st_perm);
    let fd = Unix.openfile(dest, [Unix.O_WRONLY, Unix.O_CREAT, Unix.O_TRUNC], st_perm);
    let buffer_size = 8192;
    let buffer = Bytes.create(buffer_size);
    let rec copy_loop = () => switch(Unix.read(fs, buffer, 0, buffer_size)) {
    | 0 => ()
    | r => {
      ignore(Unix.write(fd, buffer, 0, r)); copy_loop();
    }
    };
    copy_loop();
    Unix.close(fs);
    Unix.close(fd);
  }
};

let exists = path => switch (maybeStat(path)) {
| None => false
| Some(_) => true
};

let readdir = (dir) => {
  let maybeGet = (handle) =>
    try (Some(Unix.readdir(handle))) {
    | End_of_file => None
    };
  let rec loop = (handle) =>
    switch (maybeGet(handle)) {
    | None => Unix.closedir(handle); []
    | Some(name) => [name, ...loop(handle)]
    };
  loop(Unix.opendir(dir))
};

let copyDirShallow = (source, dest) => {
  readdir(source)
  |> List.filter(name => name.[0] != '.')
  |> List.iter((name) => copy(Filename.concat(source, name), Filename.concat(dest, name)))
};

let rec mkdirp = (dest) => {
  if (!exists(dest)) {
    let parent = Filename.dirname(dest);
    mkdirp(parent);
    Unix.mkdir(dest, 0o740);
  }
};

let rec copyDeep = (source, dest) => {
  if (!exists(dest)) Unix.mkdir(dest, 0o740);
  readdir(source)
  |> List.filter(name => name != "." && name != "..")
  |> List.iter((name) => {
    let src = Filename.concat(source, name);
    switch (maybeStat(src)) {
    | None => ()
    | Some({Unix.st_kind: Unix.S_DIR}) => copyDeep(src, Filename.concat(dest, name))
    | Some({Unix.st_kind: Unix.S_REG}) => copy(src, Filename.concat(dest, name))
    | _ => print_endline("Skipping " ++ src)
    }
  })
};

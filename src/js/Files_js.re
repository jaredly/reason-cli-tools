
type stat;

[@bs.module "fs"] external existsSync: string => bool = "";
[@bs.module "fs"] external rmdirSync: string => unit = "";
[@bs.module "fs"] external readdirSync: string => array(string) = "";
[@bs.module "fs"] external readFileSync: string => string => string = "";
[@bs.module "fs"] external writeFileSync: string => string => string => unit = "";
[@bs.module "fs"] external statSync: string => stat = "";
[@bs.module "fs"] external mkdirSync: string => unit = "";
[@bs.module "fs"] external unlinkSync: string => unit = "";
[@bs.send] external isFile: stat => bool = "";
[@bs.send] external isDirectory: stat => bool = "";

[@bs.module "path"] external dirname: string => string = "";
[@bs.module "path"] external join: string => string => string = "";

let rec removeDeep = path => {
  if (existsSync(path)) {
    let stat = statSync(path);
    if (isDirectory(stat)) {
      Array.iter(
        name => removeDeep(join(path, name)),
        readdirSync(path)
      );
      rmdirSync(path);
    } else {
      unlinkSync(path)
    }
  }
};

let exists = existsSync;
let isFile = path => try (isFile(statSync(path))) { | _ => false };
let isDirectory = path => try (isDirectory(statSync(path))) { | _ => false };
let rec mkdirp = path => {
  if (!exists(path)) {
    mkdirp(dirname(path));
    mkdirSync(path);
  }
};

let readDirectory = path => Array.to_list(readdirSync(path));
let readFile = path => try (Some(readFileSync(path, "utf8"))) { | e => {Js.log(e); None} };
let writeFile = (path, contents) => try {writeFileSync(path, contents, "utf8"); true} { | _ => false };

let copy = (~source, ~dest) => {
  switch (readFile(source)) {
  | None => false
  | Some(text) => writeFile(dest, text)
  }
};

let rec copyDeep = (~source, ~dest) => {
  mkdirp(dirname(dest));
  if (isFile(source)) {
    copy(~source, ~dest) |> ignore
  } else if (isDirectory(source)) {
    Array.iter(
      name => copyDeep(~source=join(source, name), ~dest=join(dest, name)),
      readdirSync(source)
    );
  }
};


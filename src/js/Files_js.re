
type stat;

[@bs.module "fs"] external existsSync: string => bool = "";
[@bs.module "fs"] external readdirSync: string => array(string) = "";
[@bs.module "fs"] external readFileSync: string => string = "";
[@bs.module "fs"] external writeFileSync: string => string => string = "";
[@bs.module "fs"] external statSync: string => stat = "";
[@bs.module "fs"] external mkdir: string => unit = "";
[@bs.module "fs"] external unlink: string => unit = "";
[@bs.send] external isFile: stat => bool = "";
[@bs.send] external isDirectory: stat => bool = "";

[@bs.module "path"] external dirname: string => string = "";
[@bs.module "path"] external join: string => string => string = "";

let removeDeep = path => {
  if (existsSync(path)) {
    let stat = statSync(path);
    if (isDirectory(stat)) {
      Array.iter(
        name => removeDeep(join(path, name)),
        readdirSync(path)
      )
    } else {
      unlink(path)
    }
  }
};

let exists = existsSync;
let isFile = path => try (isFile(statSync(path))) { | _ => false };
let isDirectory = path => try (isDirectory(statSync(path))) { | _ => false };
let rec mkdirp = path => {
  if (!exists(path)) {
    mkdirp(dirname(path))
    mkdir(path);
  }
};

let readDirectory = path => Array.to_list(readdirSync(path));
let readFile = path => try (Some(readFileSync(path))) { | _ => None };
let writeFile = (path, contents) => writeFileSync(path, contents, "utf8");

let copy: (~source: string, ~dest: string) => bool;
let copyDeep: (~source: string, ~dest: string) => unit;


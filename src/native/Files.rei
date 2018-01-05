
let exists: string => bool;
let isFile: string => bool;
let isDirectory: string => bool;
let mkdirp: string => unit;

let readDirectory: string => list(string);
let readFile: string => option(string);
let writeFile: (string, string) => bool;
let removeDeep: string => unit;

let copy: (~source: string, ~dest: string) => bool;
let copyDeep: (~source: string, ~dest: string) => unit;


let mime_for_name = ext => switch (String.lowercase(ext)) {
| "txt" => "text/plain"
| "html" => "text/html"
| "json" => "text/json"
| "js" => "application/javascript"
| "jpg" | "jpeg" => "image/jpeg"
| "png" => "image/png"
| "pdf" => "image/pdf"
| "ico" => "image/ico"
| "gif" => "image/gif"
| _ => "application/binary"
};

let ext = path => {
  let parts = Str.split(Str.regexp("\\."), path);
  List.nth(parts, List.length(parts) - 1)
};

open BasicServer.Response;
let serveStatic = (full_path, path) => {
  switch (Unix.stat(full_path)) {
  | exception Unix.Unix_error(Unix.ENOENT, _, _) => Bad(404, "File not found: " ++ path)
  | stat =>
  switch (stat.Unix.st_kind) {
  | Unix.S_REG => Ok(mime_for_name(ext(path)), Files.readFile(full_path))
  | Unix.S_DIR => {
    let index = Filename.concat(full_path, "index.html");
    if (isFile(index)) {
      Ok("text/html", readFile(index))
    } else {
      Ok("text/plain", "Directory")
    }
  }
  | _ => Bad(404, "Unexpected file type: " ++ path)
  };
  }
};

let handler = (base, method, path, headers) => {
  switch (method) {
  | "GET" => {
    let full_path = Filename.concat(base, "." ++ path);
    serveStatic(full_path, path)
  }
  | _ => Bad(401, "Method not allowed: " ++ method)
  }
};

let run = (~poll=?, ~port, path) => BasicServer.listen(~poll=?poll, port, handler(path));
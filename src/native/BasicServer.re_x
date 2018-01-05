
let recv = (client, maxlen) => {
  let bytes = Bytes.create(maxlen);
  let len = Unix.recv(client, bytes, 0, maxlen, []);
  Bytes.sub_string(bytes, 0, len)
};

let parse_top = top => {
  let parts = Str.split(Str.regexp("[ \t]+"), top);
  switch (parts) {
  | [method, path, ...others] => Some((method, path))
  | _ => None
  }
};

module StringMap = Map.Make(String);

let parse_headers = headers => {
  List.fold_left(
    (map, line) => {
      let parts = Str.split(Str.regexp(":"), line);
      switch parts {
      | [] | [_] => map
      | [name, ...rest] => StringMap.add(name, String.concat(":", rest), map)
      }
    },
    StringMap.empty,
    headers
  )
};

let parse_request = text => {
  let items = Str.split(Str.regexp("\r?\n"), text);
  switch items {
  | [] => failwith("Invalid request")
  | [top, ...headers] =>
  switch (parse_top(top)) {
  | None => failwith("Invalid top: " ++ top)
  | Some((method, path)) =>
  let header_map = parse_headers(headers);
  (method, path, header_map)
  }
  }
};

module Response = {
  type response =
    | Ok(string, string) /* mime, text */
    | Bad(int, string); /* code, text */
};
open Response;

let format_response = response => {
  let (top, body) = switch (response) {
  | Ok(mime, body) => {
    ("HTTP/1.1 200 Ok\nContent-type: " ++ mime, body)
  }
  | Bad(code, body) => {
    ("HTTP/1.1 " ++ string_of_int(code) ++ " Error\nContent-type: text/plain", body)
  }
  };
  top ++ "\nServer: Ocaml-Cross-Mobile\nContent-length: " ++ string_of_int(String.length(body)) ++ "\n\n" ++ body
};

let canRead = desc => {
  let (r, w, e) = Unix.select([desc], [], [], 0.1);
  r != []
};

let listen = (~poll=?, port, handler) => {
  let sock = Unix.socket(Unix.PF_INET, Unix.SOCK_STREAM, 0);
  Unix.setsockopt(sock, Unix.SO_REUSEADDR, true);
  Unix.bind(sock, Unix.ADDR_INET(Unix.inet_addr_any, port));
  Unix.listen(sock, 1000);

  let pollSock = () => {
    if (canRead(sock)) {
      let (client, source) = Unix.accept(sock);
      let request = recv(client,  1024);
      let response = try {
        let (method, path, header_map) = parse_request(request);

        handler(method, path, header_map);
      } {
        | _ => Bad(500, "Server error")
      };

      let response = format_response(response);
      let total = String.length(response);
      let left = ref(String.length(response));
      while (left^ > 0) {
        left := left^ - Unix.send(client, response, total - left^, left^, []);
      };
      Unix.shutdown(client, Unix.SHUTDOWN_ALL);
    }
  };

  while (true) {
    pollSock();

    switch(poll) {
    | Some(fn) => fn();
    | None => ()
    };
  }
};


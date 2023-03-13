use structopt::StructOpt;

#[derive(StructOpt)]
#[structopt(name = "yippy", about = "A LISP like interpreted language.")]
struct Opt {
    #[structopt(short, long, help = "Prints help information")]
    help: bool,

    #[structopt(short, long, help = "Prints version information")]
    version: bool,

    #[structopt(short, long, help = "Evaluate the S-Expression")]
    eval: Option<String>,

    #[structopt(short, long, help = "Evaluate specified file")]
    file: Option<String>,
}

const HELP_TEXT: &str = "A LISP like interpreted language.

USAGE:
    yippy [ARGUEMENTS]

ARGUEMENTS:
    -h, --help            Prints help information
    -v, --version         Prints version information
    -e, --eval            Evaluate the S-Expression
    -f, --file            Evaluate specified file";

fn main() {
    let opt = Opt::from_args();

    if opt.version {
        println!("{} {}", env!("CARGO_PKG_NAME"), env!("CARGO_PKG_VERSION"));
        println!("Author: {}", env!("CARGO_PKG_AUTHORS"));
    } else if opt.help {
        println!("{}", HELP_TEXT);
    } else if let Some(file) = opt.file {
        if file.ends_with(".yy") {
            eval_file(file);
        } else {
            eprintln!("\x1B[1;33mFile name must end with 'yy'.\x1B[0m");
        }
    } else if let Some(expr) = opt.eval {
        eval_inline(expr);
    } else {
        eval_line();
    }
}

fn eval_line() {
    println!("eval_line()");
}

fn eval_inline(expr: String) {
    println!("eval_inline({})", expr);
}

fn eval_file(file: String) {
    println!("eval_file({})", file);
}

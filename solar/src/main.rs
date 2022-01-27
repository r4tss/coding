use std::io;
use tui::{backend::CrosstermBackend, Terminal};

const G: f32 = (10^-11) as f32 * 6.674;
const PI: f32 = 3.14159;

struct Body
{
    x: f32,
    y: f32,
    x_vel: f32,
    y_vel: f32,
    mass: f32,
}

fn main() -> Result<(), io::Error>
{
    let stdout = io::stdout();
    let backend = CrosstermBackend::new(stdout);
    let mut term = Terminal::new(backend)?;
    let mut Bodies: Vec<Body> = vec![];

    let earth = Body { x: 1.0, y: 2.0, x_vel: 0.0, y_vel: 0.0, mass: 30.0};
    Bodies.push(earth);

    for i in 1..Bodies.len() // Iterate through bodies and apply gravitational pull for all bodies
    {
        for j in 1..Bodies.len()
        {
        }
    }

    println!("length: {}, last x: {}", Bodies.len(), Bodies[Bodies.len() - 1].x);

    Ok(())
}

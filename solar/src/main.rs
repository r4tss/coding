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

fn main()
{
    // TODO: Find a suitable graphics library
    let mut Bodies: Vec<Body> = vec![];

    let earth = Body { x: 1.0, y: 2.0, x_vel: 0.0, y_vel: 0.0, mass: 30.0};

    for i in 1..Bodies.len() // Iterate through bodies and apply gravitational pull for all bodies
    {
        for j in 1..Bodies.len()
        {
        }
    }

    println!("length: {}, last x: {}", Bodies.len(), Bodies[Bodies.len() - 1].x);
}

//This project will simulate a solar system. 

struct Vector {
    x: i32,
    y: i32,
}

struct Planet {
    x: u32,
    y: u32,
    mass: u32,
    vel: Vector,
    f: Vector,
}

fn main() {
    let planet = Planet {
        x: 0,
        y: 0,
        mass: 1000,
        vel: {0, 0},
        f: {0, 0},
    };
}

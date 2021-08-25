const GRAV_CONST: f64 = 0.00000000006674;
const PI: f64 = 3.14159;

struct Body {
    mass: f64,
    x: f64,
    y: f64,
    acc: f64
}

fn grav_pull(m1: f64, x1: f64, y1: f64, m2: f64, x2: f64, y2: f64) -> (f64, f64) {
    let x_diff = if x1 > x2 { x1 - x2 } else { x2 - x1 };
    let y_diff = if y1 > y2 { y1 - y2 } else { y2 - y1 };
    let r_squared = x_diff * x_diff + y_diff * y_diff;
    let v1 = atan(y_diff / x_diff);
    let v2 = 90 - v1;
    let v1 = if x1 > x2 && y1 < y2 { v1 + PI / 4 } else if x1 < x2 && y1 < y2 { v1 + PI / 2 } else if x1 < x2 && y1 > y2 { v1 + 3 * PI / 4 };
    (GRAV_CONST * m1 / r_squared, GRAV_CONST * m2 / r_squared)
}

fn main() {
    let earth = Body {
        mass: 5.0,
        x: 0.0,
        y: 0.0,
    };
    
    let sun = Body {
        mass: 50.0,
        x: 5.0,
        y: 5.0,
    };

    let (earth) = gravity(earth.mass, earth.x, earth.y, sun.mass, sun.x, sun.y);
}

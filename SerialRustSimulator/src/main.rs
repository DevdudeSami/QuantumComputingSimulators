extern crate num_complex;
#[macro_use] extern crate itertools;
extern crate rand;

use num_complex::Complex;
use rand::distributions::{Weighted, WeightedChoice, IndependentSample};

fn c(real: f64, imag: f64) -> Complex<f64> {
    return Complex::new(real, imag);
}

fn qubit_states_combinations(n: u32) -> Vec<String> {
    let mut results = vec!["0".to_string(), "1".to_string()];

    for _ in 0..n-1 {
        let mut new_results = Vec::new();

        for (a,b) in iproduct!(results.clone(), ["0","1"].iter()) {
            new_results.push(vec![a.to_string(),b.to_string()].join(""));
        }
        results = new_results;
    }

    return results;
}

fn main() {
    println!("Hello, world!");

    let state_vector = StateVector::new(vec![c(1.0/2.0,0.0), c(1.0/2.0,0.0), c(1.0/2.0,0.0), c(1.0/2.0,0.0)], vec![0,1]);
    println!("{}", state_vector.measure().1);

    qubit_states_combinations(4);
}

struct StateVector {
    n: u32,
    amplitudes: Vec<Complex<f64>>,
    register_indices: Vec<u32>
}

impl StateVector {
    fn new(amplitudes: Vec<Complex<f64>>, register_indices: Vec<u32>) -> StateVector {
        let n = (amplitudes.len() as f32).log2() as u32;
        assert!(register_indices.len() as u32 == n);

        let normalisation_check = (amplitudes.iter().fold(0.0, |sum, x| sum + x.norm_sqr()) * 100.0).round() / 100.0;
        println!("{:?}", normalisation_check);
        assert!(normalisation_check == 1.0);

        return StateVector { n, amplitudes, register_indices };
    }

    fn probabilities(&self) -> Vec<f64> {
        return self.amplitudes.iter().map(|a| a.norm_sqr()).collect();
    }

    fn measure(&self) -> (String, String) {
        let states = qubit_states_combinations(self.n);

        let mut items: Vec<Weighted<_>> = states.iter().zip(self.probabilities().iter()).map(|x| Weighted { item: x.0, weight: ((*x.1) * 100000.0) as u32 } ).collect();
        let wc = WeightedChoice::new(&mut items);
        let mut rng = rand::thread_rng();

        let result = wc.ind_sample(&mut rng);

        return (result.to_string(), (vec!["|", result, ">"]).join(""));
    }
}

impl ToString for StateVector {
    fn to_string(&self) -> String {
        return format!("A state vector meme.");
    }
}
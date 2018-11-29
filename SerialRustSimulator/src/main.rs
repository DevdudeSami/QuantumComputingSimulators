extern crate num_complex;
#[macro_use] extern crate itertools;
extern crate rand;
extern crate nalgebra;
extern crate time;

use num_complex::Complex;
use rand::distributions::{Weighted, WeightedChoice, IndependentSample};
use nalgebra::{DVector, Unit, DMatrix, Matrix};
use std::ops::Range;
use time::precise_time_ns;

fn c(real: f64, imag: f64) -> Complex<f64> {
    return Complex::new(real, imag);
}

fn vec(v: Vec<Complex<f64>>) -> DVector<Complex<f64>> {
    return DVector::from_iterator(v.len(), v);
}

fn qubit_states_combinations(n: u32) -> Vec<String> {
    let mut results = vec!["0".to_string(), "1".to_string()];

    for _ in 0..n-1 {
        let mut new_results = Vec::new();

        for (a,b) in iproduct!(results, ["0","1"].iter()) {
            new_results.push(vec![a.to_string(),b.to_string()].join(""));
        }
        results = new_results;
    }

    return results;
}

fn main() {
    // let mut H = DMatrix::from_iterator(2,2,vec![c(1.0/(2f64).sqrt(),0.0),c(1.0/(2f64).sqrt(),0.0),c(1.0/(2f64).sqrt(),0.0),c(-1.0/(2f64).sqrt(),0.0)]);

    let mut H: DMatrix<f64> = DMatrix::new_random(64,64);
    // let mut H2 = H.clone(); 

    // let mut state_vector_1 = StateVector::new(vec(vec![c(0.0,0.0), c(1.0,0.0)]), vec![0]);
    // let mut state_vector_2 = StateVector::new(vec(vec![c(1.0,0.0), c(0.0,0.0)]), vec![0]);
    // let mut state_vector_3 = StateVector::new(vec(vec![c(1.0,0.0), c(0.0,0.0)]), vec![0]);
    // let mut state_vector_4 = StateVector::new(vec(vec![c(1.0,0.0), c(0.0,0.0)]), vec![0]);

    // let q_reg = QRegister::new(vec![state_vector_1, state_vector_2, state_vector_3, state_vector_4]);
    // println!("{}", q_reg.state_vector().measure().1);

    let init_time = precise_time_ns();
    for _ in (0..1000000) {
        H = H.clone() * H;
        H = H.normalize();
        
        // let newM = &mut (H.clone());
        // H.mul_to(&H, newM);
        // H = newM.clone();
    }
    // println!("{:?}", H);
    let final_time = precise_time_ns();
    println!("{}", ((final_time-init_time) as f64)/1e9f64);

}

struct StateVector {
    n: u32,
    amplitudes: DVector<Complex<f64>>,
    register_indices: Vec<u32>
}

impl Clone for StateVector {
    fn clone(&self) -> StateVector {
        return StateVector { n: self.n, amplitudes: self.amplitudes.clone(), register_indices: self.register_indices.clone() };
    }
}

impl StateVector {
    fn new(amplitudes: DVector<Complex<f64>>, register_indices: Vec<u32>) -> StateVector {
        let n = (amplitudes.len() as f32).log2() as u32;
        assert!(register_indices.len() as u32 == n);

        let normalisation_check = (amplitudes.iter().fold(0.0, |sum, x| sum + x.norm_sqr()) * 100.0).round() / 100.0;
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

    fn applyGate(&mut self, G: DMatrix<Complex<f64>>) {
        self.amplitudes = G * self.amplitudes.clone();
    } 
}

impl ToString for StateVector {
    fn to_string(&self) -> String {
        return format!("A state vector.");
    }
}



struct QRegister {
    n: u32,
    vectors: Vec<StateVector>
}

impl QRegister {

    fn new(vectors: Vec<StateVector>) -> QRegister {
        let n = vectors.iter().map(|v| v.register_indices.len() as u32).sum();
        return QRegister { n, vectors };
    }

    fn state_vector(&self) -> StateVector {

        if(self.vectors.len() == 1) { return self.vectors[0].clone(); }

        let mut state_vector = self.vectors[0].amplitudes.kronecker(&self.vectors[1].amplitudes);
        for vector in self.vectors[2..].iter() {
            state_vector = state_vector.kronecker(&vector.amplitudes);
        }
        return StateVector::new(state_vector, (0u32..self.n).collect());

    }
    
}


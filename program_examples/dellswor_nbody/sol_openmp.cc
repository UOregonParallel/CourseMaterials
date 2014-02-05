#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Classical solar system model

double G = -6.674e-11; // Gravitational constant in SI m^3 kg^-1 s^-2
double time_delta = 1.0; // time per step in seconds

// Structs to make the logic a little easier
struct vector {
	double x;
	double y;
};

struct pointMass {
	double mass; // kg
	struct vector loc; // m
	struct vector vel; // m/s
};

// Vector Operations
double vector_prod(struct vector *a, struct vector *b) {
	return a->x * b->x + a->y * b->y;
}

struct vector vector_diff(struct vector *a, struct vector *b) {
	struct vector ret;
	ret.x = (a->x) - (b->x);
	ret.y = (a->y) - (b->y);
	return ret;
}

double vector_len(struct vector *a, struct vector *b) {
	struct vector d = vector_diff(a,b);
	return sqrt(vector_prod(&d,&d));
}

struct vector vector_unit(struct vector *a, struct vector *b) {
	struct vector ret = vector_diff(a,b);
	double len = vector_len(a,b);
	ret.x = ret.x/len;
	ret.y = ret.y/len;
	return ret;
}

struct vector vector_mult(double a, struct vector *v) {
	struct vector ret;
	ret.x = a*(v->x);
	ret.y = a*(v->y);
	return ret;
}

struct vector vector_add(struct vector *a, struct vector *b) {
	struct vector ret;
	ret.x = a->x + b->x;
	ret.y = a->y + b->y;
	return ret;
}

struct vector force(struct pointMass *a, struct pointMass *b) {
	double m1 = a->mass;
	double m2 = b->mass;
	double r = vector_len(&(a->loc),&(b->loc));
	struct vector unit = vector_unit(&(a->loc), &(b->loc));
	return vector_mult( (G*m1*(m2/(r*r))), &unit );
}

// Pretty printer for solar bodies
void printCoords(int nMasses, struct pointMass* masses, struct pointMass* last) {
	for(int i=0; i<nMasses; i++) {
		printf("%d %f %f %f %f\n", i, masses[i].loc.x, masses[i].loc.y, masses[i].vel.x, masses[i].vel.y);
	}
}

// Step the simulation one time delta
void stepMasses(int nMasses, struct pointMass* in_masses, struct pointMass* out_masses) {
	// Update each pointMass
	#pragma omp parallel for
	for(int i=0; i<nMasses; i++) {
		// Compute force vector
		struct vector fsum;
		fsum.x = 0.0;
		fsum.y = 0.0;
		for(int j=0; j<nMasses; j++) {
			if(i!=j) {
				struct vector cforce = force(&in_masses[i], &in_masses[j]);
				fsum = vector_add(&fsum, &cforce);
			}
		}
		struct vector vacc = vector_mult(1.0/in_masses[i].mass, &fsum);
		struct vector vadd = vector_mult(time_delta, &vacc);
		out_masses[i].vel = vector_add( &(in_masses[i].vel), &vadd);
		struct vector dis = vector_mult(time_delta, &(out_masses[i].vel));
		out_masses[i].loc = vector_add( &(in_masses[i].loc), &dis);
	}
	printCoords(nMasses, out_masses, in_masses);
}

// Run the simulation for some number of steps
void sim(int steps) {
	// Setup the solar system
	struct pointMass* in_masses;
	struct pointMass* out_masses;
	
	in_masses = (pointMass*)calloc(9, sizeof(struct pointMass));
	out_masses = (pointMass*)calloc(9, sizeof(struct pointMass));
	
	struct pointMass p;
	// sol
	p.mass = 1.9891e30;
	p.loc.x = 0.0;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 0.0;
	in_masses[0] = p;
	
	// Mercury
	p.mass = 3.3022e23;
	p.loc.x = 57e9;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 47.89e3;
	in_masses[1] = p;
	
	// Venus
	p.mass = 4.8685e24;
	p.loc.x = 108e9;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 35.03e3;
	in_masses[2] = p;
	
	// Earth
	p.mass = 5.9736e24;
	p.loc.x = 150e9;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 29.79e3;
	in_masses[3] = p;
	
	// Mars
	p.mass = 6.4185e23;
	p.loc.x = 228e9;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 24.13e3;
	in_masses[4] = p;
	
	// Jupiter
	p.mass = 1.8986e27;
	p.loc.x = 779e9;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 13.06e3;
	in_masses[5] = p;
	
	// Saturn
	p.mass = 5.6846e26;
	p.loc.x = 1.43e12;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 9.64e3;
	in_masses[6] = p;
	
	// Uranus
	p.mass = 8.681e25;
	p.loc.x = 2.88e12;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 6.81e3;
	in_masses[7] = p;
	
	// Neptune
	p.mass = 1.0243e26;
	p.loc.x = 4.5e12;
	p.loc.y = 0.0;
	p.vel.x = 0.0;
	p.vel.y = 5.43e3;
	in_masses[8] = p;
	
	
	int nbodies = 9;
	
	for(int i=0; i<nbodies; i++) {
		out_masses[i].mass = in_masses[i].mass;
	}
	
	struct pointMass *t;
	for(int i=0; i<steps; i++) {
		stepMasses(nbodies, in_masses, out_masses);
		
		t = in_masses;
		in_masses = out_masses;
		out_masses = t;
		
	}
	
	free(in_masses);
	free(out_masses);
}

// Main
int main(int argc, char** argv) {
	sim(365*60*60*24); // run the sim for about an earth year
	return 0;
}

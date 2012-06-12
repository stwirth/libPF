#include <iostream>
#include <iomanip>

#include <libPF/ParticleFilter.h>

#include "MyObservationModel.h"
#include "MyMovementModel.h"
#include "MyStateDistribution.h"

using namespace std;
int main(int, char**) {

  MyObservationModel om;
  MyMovementModel mm;

  typedef libPF::ParticleFilter<float> ParticleFilterType;

  ParticleFilterType pf(500, &om, &mm);

  cout << "Particle Filter constructed. There are " << pf.numParticles() << " particles in the filter." << endl;

  // getters of models
  libPF::ObservationModel<float>* currentObservationModel = pf.getObservationModel();
  cout << (currentObservationModel == &om) << endl; // should be "1"
  libPF::MovementModel<float>* currentMovementModel = pf.getMovementModel();
  cout << (currentMovementModel == &mm) << endl;    // should be "1"

  // setters of models
  MyObservationModel om2;
  MyMovementModel mm2;
  pf.setObservationModel(&om2);
  pf.setMovementModel(&mm2);

  // change the resampling mode
  if (pf.getResamplingMode() == libPF::RESAMPLE_NEFF)
  {
      pf.setResamplingMode(libPF::RESAMPLE_ALWAYS);
  }

  // getter for resampling strategy
  libPF::ResamplingStrategy<float>* resamplingStrategy = pf.getResamplingStrategy();

  // setter for resampling strategy (not implemented here)
  // MyResamplingStrategy myResamplingStrategy;
  // pf.setResamplingStrategy(&myResamplingStrategy);

  // to integrate a known prior state use
  pf.setPriorState(1.4);

  // or use a distribution to draw from
  MyStateDistribution distribution;
  pf.drawAllFromDistribution(distribution);

  for (int i = 0; i < 10000; i++) {
    pf.filter();
    float best = pf.getBestState();
    float mmse = pf.getMmseEstimate();
    float best5percent = pf.getBestXPercentEstimate(5.0);
    if (i % 1000 == 0)
      cout << " best: " << setiosflags(ios::fixed) << setprecision(10) << best << " (" << setiosflags(ios::fixed) << setprecision(10) << best*best << ") " <<
              " mmse: " << setiosflags(ios::fixed) << setprecision(10) << mmse << " (" << setiosflags(ios::fixed) << setprecision(10) << mmse*mmse << ") " <<
              " best 5%: " << setiosflags(ios::fixed) << setprecision(10) << best5percent << " (" << setiosflags(ios::fixed) << setprecision(10) << best5percent*best5percent << ") " <<
              endl;
  }

  // to reset the timer call
  pf.resetTimer();

  // you can call the filter steps by hand
  double dt = 0.001;
  pf.resample();
  pf.drift(dt);
  pf.diffuse(dt);
  pf.measure();

  // iteration over the particle list
  ParticleFilterType::ConstParticleIterator iter;
  unsigned int count = 0;
  for (iter = pf.particleListBegin(); iter != pf.particleListEnd(); ++iter)
  {
      libPF::Particle<float>* particle = *iter;
      cout << "Particle [" << count++ << "]: weight = " << particle->getWeight() << ", value = " << particle->getState() << endl;
  }

  cout << endl;

  // you can iterate over the list with indices as well and request weight and state from the particle filter
  for (unsigned int index = 0; index < pf.numParticles(); index += 100)
  {
      cout << "Particle [" << index << "]: weight = " << pf.getWeight(index) << ", value = " << pf.getState(index) << endl;
  }

  // number of effective particles
  cout << "Number of effective particles: " << pf.getNumEffectiveParticles() << endl;

  return 0;
}


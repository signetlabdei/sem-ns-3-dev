/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Timo Bingmann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Timo Bingmann <timo.bingmann@student.kit.edu>
 */

#include "ns3/propagation-loss-model.h"
#include "ns3/jakes-propagation-loss-model.h"
#include "ns3/constant-position-mobility-model.h"

#include "ns3/config.h"
#include "ns3/command-line.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/pointer.h"
#include "ns3/gnuplot.h"
#include "ns3/simulator.h"

#include <map>

using namespace ns3;

static void
Test (Ptr<PropagationLossModel> model, unsigned int samples = 10)
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();

  double txPowerDbm = +20; // dBm

  {
    a->SetPosition (Vector (0.0, 0.0, 0.0));

    for (double distance = 100.0; distance < 2000.0; distance += 100.0)
      {
        for (unsigned int samp = 0; samp < samples; ++samp)
          {
            b->SetPosition (Vector (distance, 0.0, 0.0));

            // CalcRxPower() returns dBm.
            double rxPowerDbm = model->CalcRxPower (txPowerDbm, a, b);

            std::cout << distance << " " << rxPowerDbm << std::endl;

            Simulator::Stop (Seconds (1.0));
            Simulator::Run ();
          }

      }
  }
}

int main (int argc, char *argv[])
{
  std::string lossModel = "Friis";
  int samples = 10;
  CommandLine cmd;
  cmd.AddValue ("LossModel", "Loss model to use", lossModel);
  cmd.AddValue ("Samples", "Number of samples for each distance", samples);
  cmd.Parse (argc, argv);

  if (lossModel == "Friis")
  {
    Ptr<FriisPropagationLossModel> friis = CreateObject<FriisPropagationLossModel> ();

    Test (friis, samples);
  }
  else if (lossModel == "LogDistance")
  {
    Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
    log->SetAttribute ("Exponent", DoubleValue (2.5));

    Test (log, samples);
  }
  else if (lossModel == "Random")
  {
    Ptr<RandomPropagationLossModel> random = CreateObject<RandomPropagationLossModel> ();
    Ptr<ExponentialRandomVariable> expVar = CreateObjectWithAttributes<ExponentialRandomVariable> ("Mean", DoubleValue (50.0));
    random->SetAttribute ("Variable", PointerValue (expVar));

    Test (random, samples);
  }
  else if (lossModel == "ThreeLogDistance")
  {
    Ptr<ThreeLogDistancePropagationLossModel> log3 = CreateObject<ThreeLogDistancePropagationLossModel> ();

    Test (log3, samples);
  }
  else if (lossModel == "ThreeLogDistanceDifferentExponents")
  {
    Ptr<ThreeLogDistancePropagationLossModel> log3 = CreateObject<ThreeLogDistancePropagationLossModel> ();
    // more prominent example values:
    log3->SetAttribute ("Exponent0", DoubleValue (1.0));
    log3->SetAttribute ("Exponent1", DoubleValue (3.0));
    log3->SetAttribute ("Exponent2", DoubleValue (10.0));

    Test (log3, samples);
  }
  else if (lossModel == "Nakagami")
  {
    Ptr<NakagamiPropagationLossModel> nak = CreateObject<NakagamiPropagationLossModel> ();

    Test (nak, samples);
  }
  else if (lossModel == "ThreeLogDistanceAndNakagami")
  {
    Ptr<ThreeLogDistancePropagationLossModel> log3 = CreateObject<ThreeLogDistancePropagationLossModel> ();

    Ptr<NakagamiPropagationLossModel> nak = CreateObject<NakagamiPropagationLossModel> ();
    log3->SetNext (nak);

    Test (log3, samples);
  }

  // produce clean valgrind
  Simulator::Destroy ();
  return 0;
}

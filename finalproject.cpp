#include "finalproject.h"
#include "buzz/buzzvm.h"

//static CRange<Real> STIMULUS_RANGE(0.0, 5000.0);
static int numClasses = 1536;

/****************************************/
/****************************************/

/**
 * Functor to get data from the robots
 */
struct GetRobotData : public CBuzzLoopFunctions::COperation {

   /** Constructor */
   GetRobotData(int n_classes) : m_vecClassCounts(n_classes, 1536) {}

   /** The action happens here */
   virtual void operator()(const std::string& str_robot_id,buzzvm_t t_vm) {
      /* Get the current class */
      buzzobj_t tClass = BuzzGet(t_vm, "class");
      /* Make sure it's the type we expect (an integer) */
      if(!buzzobj_isint(tClass)) {
         LOGERR << str_robot_id << ": variable 'class' has wrong type " << buzztype_desc[tClass->o.type] << std::endl;
         return;
      }
      /* Get the value */
      int nClass = buzzobj_getint(tClass);
      /* Make sure its value is correct */
      if(nClass >= numClasses) {
         LOGERR << str_robot_id << ": variable 'class' has wrong value " << nClass << std::endl;
         return;
      }
      /* Increase the task counter */
      //++m_vecClassCounts[nClass];

      /* Set the mapping */
      m_vecRobotsClasses[t_vm->robot] = nClass;

      /* Get the current thresholds */
//      BuzzTableOpen(t_vm, "threshold");
//      buzzobj_t tThreshold = BuzzGet(t_vm, "threshold");
      /* Make sure it's the type we expect (a table) */
//      if(!buzzobj_istable(tThreshold)) {
//         LOGERR << str_robot_id << ": variable 'threshold' has wrong type " << buzztype_desc[tThreshold->o.type] << std::endl;
//         return;
//      }
      /* Get the values */
//      m_vecRobotsThresholds[t_vm->robot].resize(m_vecClassCounts.size(), 0.0);
//      for(int i = 0; i < m_vecClassCounts.size(); ++i) {
         /* Get the object */
//         buzzobj_t tThresholdValue = BuzzTableGet(t_vm, i);
         /* Make sure it's the type we expect (a float) */
//         if(!buzzobj_isfloat(tThresholdValue)) {
//            LOGERR << str_robot_id << ": element 'threshold[" << i << "]' has wrong type " << buzztype_desc[tThresholdValue->o.type] << std::endl;
//         }
//         else {
            /* Get the value */
//            float fThresholdValue = buzzobj_getfloat(tThresholdValue);
            /* Set the mapping */
//            m_vecRobotsThresholds[t_vm->robot][i] = fThresholdValue;
//         }
//      }
   }

   /** Task counter */
   std::vector<int> m_vecClassCounts;
   /* Task-robot mapping */
   std::map<int,int> m_vecRobotsClasses;
   /* Robot-threshold mapping */
//   std::map<int,std::vector<float> > m_vecRobotsThresholds;
};

/****************************************/
/****************************************/

/**
 * Functor to put the stimulus in the Buzz VMs.
 */
//struct PutStimuli : public CBuzzLoopFunctions::COperation {
//
//   /** Constructor */
//   PutStimuli(const std::vector<Real>& vec_stimuli) : m_vecStimuli(vec_stimuli) {}
//
//   /** The action happens here */
//   virtual void operator()(const std::string& str_robot_id,
//                           buzzvm_t t_vm) {
//      /* Set the values of the table 'stimulus' in the Buzz VM */
//      BuzzTableOpen(t_vm, "stimulus");
//      for(int i = 0; i < m_vecStimuli.size(); ++i) {
//         BuzzTablePut(t_vm, i, static_cast<float>(m_vecStimuli[i]));
//      }
//      BuzzTableClose(t_vm);
//   }
//
//   /** Calculated stimuli */
//   const std::vector<Real>& m_vecStimuli;
//};

/****************************************/
/****************************************/

void CThresholdModel::Init(TConfigurationNode& t_tree) {
   /* Call parent Init() */
   CBuzzLoopFunctions::Init(t_tree);
   /* Parse XML tree */
   GetNodeAttribute(t_tree, "outfile", m_strOutFile);
   GetNodeAttribute(t_tree, "delta", m_fDelta);
   GetNodeAttribute(t_tree, "alpha", m_fAlpha);
   int nClasses;
   GetNodeAttribute(t_tree, "numClasses", nClasses);
   /* Create a new RNG */
   m_pcRNG = CRandom::CreateRNG("argos");
   /* Initialize the stimuli */
//   m_vecStimuli.resize(nClasses);
//   for(int i = 0; i < m_vecStimuli.size(); ++i) {
//      m_vecStimuli[i] = 50;
//   }
    LOGERR << ": init in cpp " << std::endl;

    /* Open the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);
}

/****************************************/
/****************************************/

void CThresholdModel::Reset() {
   /* Reset the stimuli */
//   for(int i = 0; i < m_vecStimuli.size(); ++i) {
//      m_vecStimuli[i] = 50;
//   }
   /* Convey the stimuli to every robot */
//   BuzzForeachVM(PutStimuli(m_vecStimuli));
    LOGERR << ": reset " << std::endl;

    /* Reset the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);

}

/****************************************/
/****************************************/

void CThresholdModel::Destroy() {
    LOGERR << ": destroy " << std::endl;

    m_cOutFile.close();
}

/****************************************/
/****************************************/

void CThresholdModel::PostStep() {
   /* Get robot data */
   GetRobotData cGetRobotData(1536);

    LOGERR << ": step " << std::endl;

//   BuzzForeachVM(cGetRobotData);
   /* Update the stimuli */
//   for(int i = 0; i < m_vecStimuli.size(); ++i) {
//      m_vecStimuli[i] += m_fDelta - m_fAlpha / GetNumRobots() * cGetRobotData.m_vecClassCounts[i];
//      STIMULUS_RANGE.TruncValue(m_vecStimuli[i]);
//   }
   /* Convey the stimuli to every robot */
//   BuzzForeachVM(PutStimuli(m_vecStimuli));
//   /* Flush data to the output file */
//   if(!cGetRobotData.m_vecRobotsThresholds.empty()) {
//      for(int i = 0; i < GetNumRobots(); ++i) {
//         m_cOutFile << GetSpace().GetSimulationClock() << "\t"
//                    << i << "\t"
//                    << cGetRobotData.m_vecRobotsClasses[i];
//         for(int j = 0; j < m_vecStimuli.size(); ++j) {
//            m_cOutFile << "\t" << cGetRobotData.m_vecRobotsThresholds[i][j];
//         }
//         m_cOutFile << std::endl;
//      }
//   }

   /* Flush data to the output file */
   if(!cGetRobotData.m_vecRobotsClasses.empty()) {
      for(int i = 0; i < GetNumRobots(); ++i) {
         m_cOutFile << GetSpace().GetSimulationClock() << "\t"
                    << i << "\t"
                    << cGetRobotData.m_vecRobotsClasses[i];
//         for(int j = 0; j < m_vecStimuli.size(); ++j) {
//            m_cOutFile << "\t" << cGetRobotData.m_vecRobotsThresholds[i][j];
//         }
            LOGERR << "Wrote: " << GetSpace().GetSimulationClock()
                                << i
                                << cGetRobotData.m_vecRobotsClasses[i]
                                << std::endl;
         m_cOutFile << std::endl;
      }
   }
    LOGERR << ": wrote to file " << std::endl;

}

/****************************************/
/****************************************/

bool CThresholdModel::IsExperimentFinished() {
   /* Feel free to try out custom ending conditions */
   return false;
}

/****************************************/
/****************************************/

int CThresholdModel::GetNumRobots() const {
   return m_mapBuzzVMs.size();
}

/****************************************/
/****************************************/

void CThresholdModel::BuzzBytecodeUpdated() {
   /* Convey the stimuli to every robot */
//   BuzzForeachVM(PutStimuli(m_vecStimuli));
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CThresholdModel, "finalproject");

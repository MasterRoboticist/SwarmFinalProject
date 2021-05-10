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
//      BuzzTableOpen(t_vm, "position");
//      buzzobj_t tPosition = BuzzGet(t_vm, "position");
//      /* Make sure it's the type we expect (a table) */
//      if(!buzzobj_istable(tPosition)) {
//         LOGERR << str_robot_id << ": variable 'position' has wrong type " << buzztype_desc[tPosition->o.type] << std::endl;
//         return;
//      }
      /* Get the values */
//      m_vecRobotsPositions[t_vm->robot].resize(2, 0.0);
//      for(int i = 0; i < 2; ++i) {
//         /* Get the object */
//         buzzobj_t tPositionValue = BuzzTableGet(t_vm, i);
//         /* Make sure it's the type we expect (a float) */
//         if(!buzzobj_isfloat(tPositionValue)) {
//            LOGERR << str_robot_id << ": element 'position[" << i << "]' has wrong type " << buzztype_desc[tPositionValue->o.type] << std::endl;
//         }
//         else {
//            /* Get the value */
//            float fPositionValue = buzzobj_getfloat(tPositionValue);
//            /* Set the mapping */
//            m_vecRobotsPositions[t_vm->robot][i] = fPositionValue;
//         }
//      }

       /* Get the current class */
       buzzobj_t tPosx = BuzzGet(t_vm, "posx");
       /* Make sure it's the type we expect (an integer) */
       if(!buzzobj_isfloat(tPosx)) {
           LOGERR << str_robot_id << ": variable 'posx' has wrong type " << buzztype_desc[tPosx->o.type] << std::endl;
           return;
       }
       /* Get the value */
       float nPosx = buzzobj_getfloat(tPosx);

       /* Set the mapping */
       m_vecRobotsPosx[t_vm->robot] = nPosx;

       /* Get the current class */
       buzzobj_t tPosy = BuzzGet(t_vm, "posy");
       /* Make sure it's the type we expect (an integer) */
       if(!buzzobj_isfloat(tPosy)) {
           LOGERR << str_robot_id << ": variable 'posy' has wrong type " << buzztype_desc[tPosy->o.type] << std::endl;
           return;
       }
       /* Get the value */
       float nPosy = buzzobj_getfloat(tPosx);

       /* Set the mapping */
       m_vecRobotsPosy[t_vm->robot] = nPosy;
   }

   /** Task counter */
   std::vector<int> m_vecClassCounts;
   /* Task-robot mapping */
   std::map<int,int> m_vecRobotsClasses;
   /* Robot-threshold mapping */
   std::map<int,float> m_vecRobotsPosx;
   std::map<int,float> m_vecRobotsPosy;


//   std::map<int,std::vector<float> > m_vecRobotsPosx;
//   std::map<int,std::vector<float> > m_vecRobotsPosy;
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
    /* Reset the output file */
   m_cOutFile.open(m_strOutFile.c_str(),
                   std::ofstream::out | std::ofstream::trunc);

}

/****************************************/
/****************************************/

void CThresholdModel::Destroy() {
    m_cOutFile.close();
}

/****************************************/
/****************************************/

void CThresholdModel::PostStep() {
   /* Get robot data */
   GetRobotData cGetRobotData(1536);

   BuzzForeachVM(cGetRobotData);
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
                    << cGetRobotData.m_vecRobotsClasses[i] << "\t"
                    << cGetRobotData.m_vecRobotsPosx[i] << "\t"
                    << cGetRobotData.m_vecRobotsPosy[i];
//         for(int j = 0; j < 2; ++j) {
//            m_cOutFile << "\t" << cGetRobotData.m_vecRobotsPositions[i][j];
//            if(i == 1) {
//                LOGERR << cGetRobotData.m_vecRobotsPositions[i][j] << std::endl;
//            }
//         }
         m_cOutFile << std::endl;
      }
   }
   else{
       LOGERR << "vecRobotClasses is empty" << std::endl;
   }
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

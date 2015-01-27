/**
 * This code has been introduced in 
 * http://www.gamedev.net/page/resources/_/technical/game-programming/bvh-file-loading-and-displaying-r3295
 * so it is licenced with GDOL (Gamedeve.net Open Licence)
 */
 
 #include "bvh.h"

Bvh::Bvh()
    : rootJoint(NULL)
{
    motionData.data = 0;
}

void deleteJoint(JOINT* joint)
{
    if( joint == NULL )
        return;

    for(JOINT* child : joint->children)
        deleteJoint(child);

    if( joint->channels_order != NULL )
        delete joint->channels_order;

//    if( joint->name != NULL && joint->name != "EndSite" ){
//        klog.l() << joint->name;
//        delete joint->name;
//    }

    delete joint;
}

Bvh::~Bvh()
{
    klog.l() << "Bvh destructor";
    deleteJoint(rootJoint);

//    for(JOINT* joint : allJoints)
//    {

//        delete joint->channels_order;
//        delete joint->name;
//        delete joint;
//    }


    if( motionData.data != NULL )
        delete[] motionData.data;
}

void Bvh::load(const std::string& filename)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios_base::in);

    if( file.is_open() )
    {
        std::string line;

        while( file.good() )
        {
            file >> line;
            klog.l("load") << line;
            if( trim(line) == "HIERARCHY" )
                loadHierarchy(file);
            break;
        }

        file.close();
    }


}

void Bvh::printJoint(const JOINT* const joint) const
{
    klog.l("joint") << "print joint" << joint->name << joint->channel_start;

    for(std::vector<JOINT*>::const_iterator ct = joint->children.begin();
        ct != joint->children.end(); ++ct)
    {
        JOINT* _tmp = *ct;

        if( _tmp->children.size() > 0 )
            printJoint(_tmp);
    }

}
/*
const Logger& operator<<(const Logger& logger, const glm::vec4& vec)
{
    return klog << vec.x << vec.y << vec.z << vec.w;
}

const Logger& operator<<(const Logger& logger, const glm::mat4& mat)
{
    for( int i = 0; i < mat.length(); i++ )
        ((Logger&)logger).l() << mat[i];
    return logger;
}
*/

// calculate local transformation matrix for specific frame - revisit this
void moveJoint(JOINT* joint, MOTION* motionData, int frame_starts_index)
{
    int start_index = frame_starts_index + joint->channel_start;

    // postavljamo prvo lokalnu mat na offset poziciju
    joint->matrix = glm::translate(glm::mat4(1.0),
                                   glm::vec3(joint->offset.x,
                                             joint->offset.y,
                                             joint->offset.z));

    for(int i = 0; i < joint->num_channels; i++)
    {
        const short& channel = joint->channels_order[i];

        float value = motionData->data[start_index + i];
        if( channel & Xposition )
        {
            joint->matrix = glm::translate(joint->matrix, glm::vec3(value, 0, 0));
        }
        if( channel & Yposition )
        {
            joint->matrix = glm::translate(joint->matrix, glm::vec3(0, value, 0));
        }
        if( channel & Zposition )
        {
            joint->matrix = glm::translate(joint->matrix, glm::vec3(0, 0, value));
        }

        if( channel & Xrotation )
        {
            joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(1, 0, 0));
        }
        if( channel & Yrotation )
        {
            joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(0, 1, 0));
        }
        if( channel & Zrotation )
        {
            joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(0, 0, 1));
        }
    }

    // mul current matrix with one of a parent
    if( joint->parent != NULL )
        joint->matrix = joint->parent->matrix * joint->matrix;

    for(auto& child : joint->children)
        moveJoint(child, motionData, frame_starts_index);
}

void Bvh::moveTo(unsigned frame)
{
    // calculate start index for specific frame for motion data
    unsigned start_index = frame * motionData.num_motion_channels;

    // skeleton transformation
    moveJoint(rootJoint, &motionData, start_index);
}

void Bvh::testOutput() const
{
    if( rootJoint == 0 )
        return;

    klog.l() << "output";
    printJoint(rootJoint);

    klog.l() << "num frames: " << motionData.num_frames;
    klog.l() << "num motion channels: " << motionData.num_motion_channels;

    int num_frames = motionData.num_frames;
    int num_channels = motionData.num_motion_channels;

//    for(int frame = 0; frame < num_frames; frame++)
//    {
//        klog.l() << "frame: " << frame;

////        for(int channel = 0; channel < num_channels; channel++)
////        {
////            int index = frame * num_channels + channel;

////            klog << motionData.data[index];
////        }

////        static int tmp = 0;
////        std::for_each(allJoints.begin(), allJoints.end(), [](const JOINT* joint){
////            tmp += joint->num_channels;

////        });



//        int tmp = frame * num_channels;
//        int tmp2 = 0;



//        for(std::set<JOINT*>::const_iterator it = allJoints.begin();
//            it != allJoints.end(); ++it)
//        {
//            JOINT* joint = *it;

//            klog.l() << joint->name;
////            if( joint->name == "EndSite" )
////                continue;

////            int motion_index = tmp + tmp2;

////            int start = motion_index;
////            int end = start + joint->num_channels;

////            klog.l() << joint->name << motion_index;
////            for(int x = start; x < end; x++)
////            {
////                //klog << motionData.data[x];
////            }

////            tmp2 += joint->num_channels;

////            float* niz = this->getMotion(frame, joint);
////            for( int i = 0; i < joint->num_channels; i++)
////            {
////                klog << niz[i];
////            }
//        }

////        klog.l("p") << frame << motionData.data[frame];
//    }

}

void Bvh::loadHierarchy(std::istream& stream)
{
    std::string tmp;

    while( stream.good() )
    {
        stream >> tmp;
        klog.l("hier") << tmp;

        if( trim(tmp) == "ROOT" )
            rootJoint = loadJoint(stream);
        else if( trim(tmp) == "MOTION" )
            loadMotion(stream);
    }
}

void Bvh::loadMotion(std::istream& stream)
{
//    klog.l() << "num joints:" << stats.num_total_joints;
//    klog << stats.num_motion_channels;

    std::string tmp;

    while( stream.good() )
    {
        stream >> tmp;

        if( trim(tmp) == "Frames:" )
        {
            stream >> motionData.num_frames;
        }
        else if( trim(tmp) == "Frame" )
        {
            float frame_time;
            stream >> tmp >> frame_time;

            int num_frames   = motionData.num_frames;
            int num_channels = motionData.num_motion_channels;

            klog.l() << "num frames:" << num_frames;
            klog.l() << "num channels:" << num_channels;

            motionData.data = new float[num_frames * num_channels];

            for( int frame = 0; frame < num_frames; frame++ )
            {
                for( int channel = 0; channel < num_channels; channel++)
                {
                    float x;
                    std::stringstream ss;
                    stream >> tmp;
                    ss << tmp;
                    ss >> x;

                    int index = frame * num_channels + channel;
                    motionData.data[index] = x;
                }
            }
        }
    }
}

JOINT* Bvh::loadJoint(std::istream& stream, JOINT* parent)
{
    JOINT* joint = new JOINT;
    joint->parent = parent;

//    stats.num_total_joints++;
//    allJoints.insert(joint);

    // load joint name
    std::string* name = new std::string;
    stream >> *name;
    joint->name = name->c_str();

    std::string tmp;
    joint->matrix = glm::mat4(1.0);

    static int _channel_start = 0;
    unsigned channel_order_index = 0;
    while( stream.good() )
    {
        stream >> tmp;
        tmp = trim(tmp);

        // setting channels
        char c = tmp.at(0);
        if( c == 'X' || c == 'Y' || c == 'Z' )
        {
            if( tmp == "Xposition" )
            {
                joint->channels_order[channel_order_index++] = Xposition;
            }
            if( tmp == "Yposition" )
            {
                joint->channels_order[channel_order_index++] = Yposition;
            }
            if( tmp == "Zposition" )
            {
                joint->channels_order[channel_order_index++] = Zposition;
            }

            if( tmp == "Xrotation" )
            {
                joint->channels_order[channel_order_index++] = Xrotation;
            }
            if( tmp == "Yrotation" )
            {
                joint->channels_order[channel_order_index++] = Yrotation;
            }
            if( tmp == "Zrotation" )
            {
                joint->channels_order[channel_order_index++] = Zrotation;
            }
        }

        if( tmp == "OFFSET" )
        {
            stream  >> joint->offset.x
                    >> joint->offset.y
                    >> joint->offset.z;
        }
        else if( tmp == "CHANNELS" )
        {
            // loading num of channels
            stream >> joint->num_channels;

            // adding to statistics
            motionData.num_motion_channels += joint->num_channels;

            // increasing static counter of channel index startin motion section
            joint->channel_start = _channel_start;
            _channel_start += joint->num_channels;

            // creating array for channel order specification
            joint->channels_order = new short[joint->num_channels];

        }
        else if( tmp == "JOINT" )
        {
            JOINT* tmp_joint = loadJoint(stream, joint);

            tmp_joint->parent = joint;
            joint->children.push_back(tmp_joint);

//            allJoints.insert(tmp_joint);

        }
        else if( tmp == "End" )
        {
            // End Site {
            stream >> tmp >> tmp;

            JOINT* tmp_joint = new JOINT;

            tmp_joint->parent = joint;
            tmp_joint->num_channels = 0;
            tmp_joint->name = "EndSite";
            joint->children.push_back(tmp_joint);

//            allJoints.insert(tmp_joint);

            stream >> tmp;
            if( tmp == "OFFSET" )
                stream >> tmp_joint->offset.x
                       >> tmp_joint->offset.y
                       >> tmp_joint->offset.z;

            // ucitavanje }
            stream >> tmp;
        }
        else if( tmp == "}" )
            return joint;

    }
}














#include "MWParticleImport.h"

using namespace Myway;

#define space           " "
#define tab             "   "
#define bracket_b       "{"
#define bracket_e       "}"

#define max_count       "max_count"
#define material        "material"
#define emitter         "emitter"
#define point           "point"
#define position        "position"
#define direction       "direction"
#define up              "up"
#define color           "color"
#define angle           "angle"
#define life            "life"
#define speed           "speed"
#define size            "size"
#define rate            "rate"
#define duration_time   "duration_time"
#define replay_time     "replay_time"

#define effector        "effector"
#define movement        "movement"
#define color_fade      "color_fade"
#define start_color     "start_color"
#define end_color       "end_color"


#define material_a      "material      "
#define max_count_a     "max_count     "
#define position_a      "position      "
#define direction_a     "direction     "
#define up_a            "up            "
#define color_a         "color         "
#define angle_a         "angle         "
#define life_a          "life          "
#define speed_a         "speed         "
#define size_a          "size          "
#define rate_a          "rate          "
#define duration_time_a "duration_time "
#define replay_time_a   "replay_time   "
#define start_color_a   "start_color   "
#define end_color_a     "end_color     "


//void ParticleSystemImport_0::Import(ParticleSystem * ps, const String & sFile)
//{
//    /*
//    ps->Destroy();
//
//    ps->SetFileName(sFile);
//
//    String sFilePath;
//
//    ResourceManager::Instance().GetFilePath(sFile, sFilePath);
//
//    CommandScript cst;
//    bool bRet = cst.LoadScriptFile(sFilePath);
//    EXCEPTION_DEBUG(bRet, "Particle Script file error.");
//
//    String cmd;
//    String param1;
//    String param2;
//    String param3;
//    String param4;
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//            param3 = cst.ParseStringParam();
//            param4 = cst.ParseStringParam();
//
//            //material
//            if (cmd == material)
//            {
//                ps->SetMaterial(param1);
//            }
//
//            //max_count
//            else if (cmd == max_count)
//            {
//                ps->SetMaxSize(param1.ToInt());
//            }
//
//            //emitter
//            else if (cmd == emitter)
//            {
//                if (param1 == point)
//                    ReadPointEmitter(ps, cst);
//            }
//
//            //effector
//            else if (cmd == effector)
//            {
//                if (param1 == movement)
//                    ReadMovementEffector(ps, cst);
//                else if (param1 == color_fade)
//                    ReadColorFadeEffector(ps, cst);
//            }
//        }
//    }
//
//    ps->Initialize();
//    */
//}
//
//void ParticleSystemImport_0::ReadPointEmitter(ParticleSystem * ps, CommandScript & cst)
//{
//    String cmd;
//    String param1;
//    String param2;
//    String param3;
//    String param4;
//
//    PointEmitter * pe = (PointEmitter*)ps->CreateEmitter(EMITTER_POINT);
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//            param3 = cst.ParseStringParam();
//            param4 = cst.ParseStringParam();
//
//            //end
//            if (cmd == bracket_e)
//            {
//                break;
//            }
//
//            //position
//            else if (cmd == position)
//            {
//                pe->SetPosition(param1.ToFloat(),
//                                param2.ToFloat(),
//                                param3.ToFloat());
//            }
//
//            //direction
//            else if (cmd == direction)
//            {
//                pe->SetDirection(param1.ToFloat(),
//                                 param2.ToFloat(),
//                                 param3.ToFloat());
//            }
//
//            //up
//            else if (cmd == up)
//            {
//                pe->SetUp(param1.ToFloat(),
//                          param2.ToFloat(),
//                          param3.ToFloat());
//            }
//
//            //color
//            else if (cmd == color)
//            {
//                pe->SetInitColor(param1.ToFloat(),
//                                 param2.ToFloat(),
//                                 param3.ToFloat(),
//                                 param4.ToFloat());
//            }
//
//            //angle
//            else if (cmd == angle)
//            {
//                pe->SetInitMaxAngle(param1.ToFloat());
//            }
//
//            //life
//            else if (cmd == life)
//            {
//                pe->SetInitLife(param1.ToFloat());
//            }
//
//            //speed
//            else if (cmd == speed)
//            {
//                pe->SetInitSpeed(param1.ToFloat());
//            }
//
//            //size
//            else if (cmd == size)
//            {
//                pe->SetInitSize(param1.ToFloat());
//            }
//
//            //rate
//            else if (cmd == rate)
//            {
//                pe->SetRate(param1.ToFloat());
//            }
//
//            //duration time
//            else if (cmd == duration_time)
//            {
//                pe->SetDurationTime(param1.ToFloat());
//            }
//
//            //replay time
//            else if (cmd == replay_time)
//            {
//                pe->SetReplayTime(param1.ToFloat());
//            }
//        }
//    }
//}
//
//void ParticleSystemImport_0::ReadMovementEffector(ParticleSystem * ps, CommandScript & cst)
//{
//    String cmd;
//    String param1;
//    String param2;
//    String param3;
//    String param4;
//
//    MovementEffector * me = (MovementEffector*)ps->CreateEffector(EFFECTOR_MOVEMENT);
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//            param3 = cst.ParseStringParam();
//            param4 = cst.ParseStringParam();
//
//            //end
//            if (cmd == bracket_e)
//            {
//                break;
//            }
//        }
//    }
//}
//
//void ParticleSystemImport_0::ReadColorFadeEffector(ParticleSystem * ps, CommandScript & cst)
//{
//    String cmd;
//    String param1;
//    String param2;
//    String param3;
//    String param4;
//
//    ColorFadeEffector * ce = (ColorFadeEffector*)ps->CreateEffector(EFFECTOR_COLOR_FADE);
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//            param3 = cst.ParseStringParam();
//            param4 = cst.ParseStringParam();
//
//            //end
//            if (cmd == bracket_e)
//            {
//                break;
//            }
//
//            //start color
//            else if (cmd == start_color)
//            {
//                ce->SetStartColor(param1.ToFloat(),
//                                  param2.ToFloat(),
//                                  param3.ToFloat(),
//                                  param4.ToFloat());
//            }
//
//            //end color
//            else if (cmd == end_color)
//            {
//                ce->SetEndColor(param1.ToFloat(),
//                                param2.ToFloat(),
//                                param3.ToFloat(),
//                                param4.ToFloat());
//            }
//        }
//    }
//}
//
//
//void ParticleSystemExport_0::Export(ParticleSystem * ps, const String & sFile)
//{
//    File file;
//
//    file.Open(sFile, OM_WRITE);
//
//    //material
//    file << material_a << ps->GetMaterial()->GetName()
//        << File::ENDL;
//
//    //max_count
//    file << max_count_a << String(ps->GetMaxSize())
//        << File::ENDL << File::ENDL;
//
//    //emitter
//    for (int i = 0; i < ps->GetEmitterCount(); ++i)
//    {
//        ParticleEmitter * pe = ps->GetEmitter(i);
//        switch (pe->GetType())
//        {
//        case EMITTER_POINT:
//            WritePointEmitter(pe, file);
//            break;
//        }
//    }
//
//    //effector
//    for (int i = 0; i < ps->GetEffectorCount(); ++i)
//    {
//        ParticleEffector * pe = ps->GetEffector(i);
//        switch (pe->GetType())
//        {
//        case EFFECTOR_MOVEMENT:
//            WriteMovementEffector(pe, file);
//            break;
//
//        case EFFECTOR_COLOR_FADE:
//            WriteColorFadeEffector(pe, file);
//            break;
//        }
//    }
//
//    file.Close();
//}
//
//void ParticleSystemExport_0::WritePointEmitter(ParticleEmitter * em, File & file)
//{
//    PointEmitter * pe = static_cast<PointEmitter*>(em);
//
//    file << emitter << space << point
//        << File::ENDL << bracket_b << File::ENDL;
//
//    //position
//    file << tab << position_a 
//        << String(pe->GetPosition().x) << space
//        << String(pe->GetPosition().y) << space
//        << String(pe->GetPosition().z) << space
//        << File::ENDL;
//
//    //direction
//    file << tab << direction_a
//        << String(pe->GetDirection().x) << space
//        << String(pe->GetDirection().y) << space
//        << String(pe->GetDirection().z) << space
//        << File::ENDL;
//
//    //up
//    file << tab << up_a
//        << String(pe->GetUp().x) << space
//        << String(pe->GetUp().y) << space
//        << String(pe->GetUp().z) << space
//        << File::ENDL;
//
//    //color
//    file << tab << color_a
//        << String(pe->GetInitColor().r) << space
//        << String(pe->GetInitColor().g) << space
//        << String(pe->GetInitColor().b) << space
//        << String(pe->GetInitColor().a) << space
//        << File::ENDL;
//
//    //angle
//    file << tab << angle_a
//        << String(pe->GetInitMaxAngle()) << space
//        << File::ENDL;
//
//    //life
//    file << tab << life_a
//        << String(pe->GetInitLife()) << space
//        << File::ENDL;
//
//    //speed
//    file << tab << speed_a
//        << String(pe->GetInitSpeed()) << space
//        << File::ENDL;
//
//    //size
//    file << tab << size_a
//        << String(pe->GetInitSize()) << space
//        << File::ENDL;
//
//    //rate
//    file << tab << rate_a
//        << String(pe->GetRate()) << space
//        << File::ENDL;
//
//    //duration time
//    file << tab << duration_time_a
//        << String(pe->GetDurationTime()) << space
//        << File::ENDL;
//
//    //replay time
//    file << tab << replay_time_a
//        << String(pe->GetReplayTime()) << space
//        << File::ENDL;
//
//    file << bracket_e << File::ENDL << File::ENDL;
//}
//
//void ParticleSystemExport_0::WriteMovementEffector(ParticleEffector * ef, File & file)
//{
//    MovementEffector * me = static_cast<MovementEffector*>(ef);
//
//    file << effector << space << movement
//        << File::ENDL << bracket_b << File::ENDL;
//
//    file << bracket_e << File::ENDL << File::ENDL;
//}
//
//void ParticleSystemExport_0::WriteColorFadeEffector(ParticleEffector * ef, File & file)
//{
//    ColorFadeEffector * me = static_cast<ColorFadeEffector*>(ef);
//
//    file << effector << space << color_fade
//        << File::ENDL << bracket_b << File::ENDL;
//
//    file << tab << start_color_a
//        << String(me->GetStartColor().r) << space
//        << String(me->GetStartColor().g) << space
//        << String(me->GetStartColor().b) << space
//        << String(me->GetStartColor().a) << space
//        << File::ENDL;
//
//    file << tab << end_color_a
//        << String(me->GetEndColor().r) << space
//        << String(me->GetEndColor().g) << space
//        << String(me->GetEndColor().b) << space
//        << String(me->GetEndColor().a) << space
//        << File::ENDL;
//
//    file << bracket_e << File::ENDL << File::ENDL;
//}
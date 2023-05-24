<?php

namespace App\Http\Controllers;

use App\Models\Event;
use App\Models\Team;
use Illuminate\Http\Request;

class ArduinoController extends Controller {

    public function receiveArduinoDataFromPython(Team $team, Request $request){

        if ($team->getScoreAttribute() != 0){
            print "error";
            die();
        }

        $data = [];
        $data['exploded'] = true;
        foreach($request["events"] as $event){
            $event["team_id"] = $team->id;
            Event::create($event);
            
            if ($event["name"] == "defused"){
                $data['exploded'] = false;
                $data['time']     = $event['time'];
            } else
            if ($event["name"] == "exploded"){
                $data['time']     = $event['time'];
            }
        }

        $team->update($data);
        print "saved";
    }
}

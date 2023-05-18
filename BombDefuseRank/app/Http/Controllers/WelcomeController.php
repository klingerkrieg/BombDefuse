<?php

namespace App\Http\Controllers;

use App\Models\Member;
use App\Models\Team;
use Illuminate\Http\Request;

class WelcomeController extends Controller
{
    
    public function index() {
        #$defuseds = Team::with('members')->orderBy('time','asc')->where("exploded",0)->get();
        #$explodeds = Team::with('members')->orderBy('time','desc')->where("exploded",1)->get();
        
        #$teams = array_merge($defuseds->toArray(),$explodeds->toArray());
        $teams = Team::with('members')->where("event_name",env("APP_EVENT_NAME"))->get()->sortByDesc("score");
        
        return view('welcome', ["teams"=>$teams]);
    }

}

<?php

namespace App\Http\Controllers;

use App\Models\Member;
use App\Models\Team;
use Illuminate\Http\Request;

class HomeController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }

    
    public function index() {
        $teams = Team::orderBy('time','desc')->get();
        return view('home', ["teams"=>$teams]);
    }

    public function new(){
        return view("form",["team"=>new Team()]);
    }

    public function save(Request $request){
        $team = Team::create($request->all());
        
        $names = $request->get("name");
        foreach($names as $key=>$name){
            if ($name != null){
                Member::create([
                    "name"=>$name,
                    "age"=>$request->get("age")[$key],
                    "course"=>$request->get("course")[$key],
                    "team_id"=>$team->id
                ]);
            }
        }

        return redirect(route("edit",$team));
    }

    public function edit(Team $team){
        return view("form",["team"=>$team, "members"=>$team->members]);
    }

    public function update(Team $team, Request $request){
        $team = Team::create($request->all());
        
        #remove todos
        foreach($team->members as $member){
            $member->delete();
        }

        #adiciona todos novamente
        $names = $request->get("name");
        foreach($names as $key=>$name){
            if ($name != null){
                Member::create([
                    "name"=>$name,
                    "age"=>$request->get("age")[$key],
                    "course"=>$request->get("course")[$key],
                    "team_id"=>$team->id
                ]);
            }
        }

        return redirect(route("edit",$team));
    }
}

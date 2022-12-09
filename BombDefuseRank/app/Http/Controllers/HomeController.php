<?php

namespace App\Http\Controllers;

use App\Models\Member;
use App\Models\Team;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

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

    public function delete(Team $team){
        #remove todos
        foreach($team->members as $member){
            $member->delete();
        }
        $team->delete();
        return redirect(route('dashboard'));
    }

    public function foto(Team $team){
        return view("foto", ["team"=>$team]);
    }

    public function edit(Team $team){

        $canDelete = Auth::user()->email == "klingerkrieg@gmail.com";

        return view("form",["team"=>$team, "members"=>$team->members,"canDelete"=>$canDelete]);
    }

    public function update(Team $team, Request $request){
        $team->update($request->all());
        
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

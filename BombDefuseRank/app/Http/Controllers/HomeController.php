<?php

namespace App\Http\Controllers;

use App\Models\Event;
use App\Models\Member;
use App\Models\Team;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Validator;

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
        $defuseds = Team::with('members')->orderBy('time','asc')->where("exploded",0)->get();
        $explodeds = Team::with('members')->orderBy('time','desc')->where("exploded",1)->get();
        
        $teams = array_merge($defuseds->toArray(),$explodeds->toArray());
        
        return view('home', ["teams"=>$teams]);
    }

    public function new(){
        $canDelete = Auth::user()->email == "klingerkrieg@gmail.com";
        
        return view("form",["team"=>new Team(),
                            "canDelete"=>$canDelete]);
    }

    protected function validator(array $data) {
        return Validator::make($data, [
            'team_name' => ['required', 'string', 'max:255'],
        ]);
    }

    public function save(Request $request){
        $this->validator($request->all())->validate();
        
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

        return view("form",["team"=>$team, 
                            "members"=>$team->members,
                            "canDelete"=>$canDelete]);
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

    public function receiveArduinoDataFromPython(Team $team, Request $request){

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

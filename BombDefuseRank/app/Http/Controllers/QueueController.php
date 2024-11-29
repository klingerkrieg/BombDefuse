<?php

namespace App\Http\Controllers;

use App\Http\Requests\TeamQueueRequest;
use Illuminate\Http\Request;
use App\Models\TeamQueue;

class QueueController extends Controller
{

    public function create()
    {
        return view('admin.teamqueue.form', [
            'equipe' => new TeamQueue(),
        ]);
    }


    public function store(TeamQueueRequest $request, TeamQueue $equipeFila)
    {
        $equipeFila->create($request->validated());
        return redirect()->route('fila');
    }

    public function edit(int $id, TeamQueue $equipeFila)
    {
        $equipe = $equipeFila->find($id);

        if(!$equipe){
            return back();
        }

        return view('admin.teamqueue.form', [
            'equipe' => $equipe
        ]);
    }

    public function update(int $id, TeamQueueRequest $request, TeamQueue $equipeFila){
        $equipe = $equipeFila->find($id);

        if(!$equipe){
            return back();
        }

        $equipe->update($request->validated());

        return redirect()->route('fila');
    }

    public function destroy(int $id, TeamQueue $equipeFila){
        $equipe = $equipeFila->find($id);

        if(!$equipe){
            return back();
        }

        $equipe->delete();
        return redirect()->route('fila');
    }
}

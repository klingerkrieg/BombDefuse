@extends('layouts.auth')

@section('title', 'Formulário de Equipes')

@section('content')

<style>

red{
    color:red;
}


</style>

<main>
    <div class="container">
        <div class="row justify-content-center">
            <div class="col-md-8">
                <div class="card">
                    <div class="card-header">{{ __('Formulário de Equipes') }}</div>
    
                    <div class="card-body">
    
                        @if ($team->id)
                            <form id="form" action="{{route('update',$team)}}" method="post">
                                @csrf
                        @else
                            <form id="form" action="{{route('save')}}" method="post">
                                @csrf
                        @endif
    
                            <h4>Código da equipe: {{$team->id}}</h4>
                            <hr/>
    
                            <div class='row'>
                                <label class='col-md'>Nome da equipe <red>*</red>
                                    <input name="team_name" class="form-control @error('team_name') is-invalid @enderror" type="text" value="{{$team->team_name}}">
    
                                    @error('team_name')
                                        <div class="alert alert-danger">{{ $message }}</div>
                                    @enderror
                                </label>
    
                                <label class='col-md-3'>Tempo
                                    <input name="time" class="form-control" type="time" max="18:00" min="00:00" value="{{$team->time}}">
                                    <span class="form-text text-muted">Tempo que a equipe levou para explodir ou desarmar.</span>
                                </label>
    
                            </div>

                            
                            @if(isset($equipesFila) && !$equipesFila->isEmpty())
                                <div class="row mb-3">
                                    <label> Equipe na Fila
                                        <select class="form-select" name="team_queue_id">
                                            <option value="" disabled {{ old('team_queue_id', $team->team_queue_id ?? '') == '' ? 'selected' : '' }}>Selecione uma equipe da fila</option>
                                            @foreach ($equipesFila as $k=>$equipe)
                                                <option value="{{ $equipe->id }}" {{ old('team_queue_id', $team->team_queue_id ?? '') == $equipe->id ? 'selected' : '' }}>
                                                    {{$k+1}}° - {{ $equipe->nome }}
                                                </option>
                                            @endforeach
                                        </select>
                                    </label>
                                </div>
                            @endif
                           
    
                            <div class='row'>
    
                                <label class='col-md-3'>
                                    <input type="hidden" name="exploded" value="0" >
                                    <input name="exploded" class="form-check-input" type="checkbox" @if ($team->exploded == 1) {{"checked"}} @endif value="1">
                                    A bomba explodiu
                                </label>
    
    
                            </div>
    
                            @for ($i = 0; $i < 5; $i++)
                            <div>
                                <h5 class="border-bottom mt-5">Membro {{$i+1}}</h5>
                                <div class='row'>
    
                                    <label class='col-md-6'> Nome <red>*</red>
                                        <input name="name[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->name ?? null}}">
                                    </>
    
                                    <label class='col-md-2'> Idade
                                        <input name="age[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->age ?? null}}">
                                    </label>
    
                                    <label class='col-md-4'> Curso
                                        <input name="course[{{$i}}]" class="form-control" type="text" value="{{$members[$i]->course ?? null}}">
                                    </label>
    
                                    {{--<label class='col-md-2'> Período
                                        <input name="course_period[{{$i}}]" class="form-control" type="text" value="">
                                    </label>
    
                                    <label class='col-md-3'> Nota para o projeto
                                        <input name="grade[{{$i}}]" class="form-control" type="text" value="">
                                    </label>--}}
                                </div>
                            </div>
                            @endfor
    
                        </form>
    
                        <h4>Detalhes - Pontuação: {{number_format($team->score,2)}} </h4>
    
                        <table class="table">
                            <thead>
                              <tr>
                                <th scope="col">Nome</th>
                                <th scope="col">Hora</th>
                                <th scope="col">Valor</th>
                                <th scope="col">Pontos</th>
                              </tr>
                            </thead>
                            <tbody>
                            @foreach ($team->events as $event)
                                <tr>
                                    <th scope="row">{{$event->name}}</th>
                                    <td>{{$event->time}}</td>
                                    <td>{{$event->type}}</td>
                                    <td>{{number_format($event->score,2)}}</td>
                                </tr>
                            @endforeach
                            </tbody>
                          </table>
    
    
                        <div class="row gap-3 mt-3 d-flex justify-content-center">
                            <button type="submit" class="btn btn-primary col-md-2" form="form">Salvar</button>
                            <a href="{{route("new")}}" class="btn btn-secondary col-md-2">Novo</a>
                            <a href="{{route("rank")}}" class="btn btn-secondary col-md-2">Rank</a>
                            @if ($team->id)
                                <a href="{{route("foto",$team)}}" class="btn btn-success col-md-2">Foto</a>

                                @can('admin')
                                    <a href="{{route("delete",$team)}}" class="btn btn-danger col-md-2">Deletar</a>
                                @endcan
                            @endif
                        </div>
    
    
                    </div>
                </div>
            </div>
        </div>
    </div>
</main>

@endsection

<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class TeamQueueRequest extends FormRequest
{

    public function authorize()
    {
        return $this->user()->hasAnyPermission(['admin', 'org']);
    }


    public function rules()
    {
        $rules = [
            'nome' => [
                'required',
            ],
            'whatsapp' => [
                'required',
            ]
        ];

        return $rules;
    }
}
